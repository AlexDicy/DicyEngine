#include "pch/enginepch.h"
#include "spherical_harmonics.h"

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyClangDiagnosticImplicitIntFloatConversion
// ReSharper disable CppClangTidyBugproneNarrowingConversions
// ReSharper disable CppClangTidyClangDiagnosticImplicitFloatConversion

//
// Heavily inspired by Google Filament's implementation
//

std::vector<glm::vec3> SphericalHarmonics::calculateSH(const Ref<CubeMap>& cubeMap, const unsigned int bands, const bool calculateForIrradiance, const bool preprocessForShaders) {
    const unsigned int numCoefficients = bands * bands;
    std::vector<glm::vec3> sh(numCoefficients);
    const unsigned int size = cubeMap->getSize();

    for (uint8_t side = 0; side < 6; side++) {
        std::vector<glm::vec3> currentSH(numCoefficients);
        std::vector<float> currentSHb(numCoefficients);

        for (unsigned int y = 0; y < size; y++) {
            for (unsigned int x = 0; x < size; x++) {
                const glm::vec3 direction = cubeMap->getPixelDirection(static_cast<CubeMap::Face>(side), x, y);
                glm::vec3 color = cubeMap->getPixel(static_cast<CubeMap::Face>(side), x, y);
                // take solid angle into account
                color *= solidAngle(size, x, y);
                calculateSHBasis(currentSHb.data(), bands, direction);
                // apply coefficients to the sampled color
                for (unsigned int i = 0; i < numCoefficients; i++) {
                    currentSH[i] += color * currentSHb[i];
                }
            }
        }

        for (unsigned int i = 0; i < numCoefficients; i++) {
            sh[i] += currentSH[i];
        }
    }

    // precompute the scaling factor K
    std::vector<float> K = calculateKi(bands);

    // apply truncated cos (irradiance)
    if (calculateForIrradiance) {
        for (unsigned int l = 0; l < bands; l++) {
            const float truncatedCosSh = calculateTruncatedCosSH(l);
            K[getSHIndex(0, l)] *= truncatedCosSh;
            for (unsigned int m = 1; m <= l; m++) {
                K[getSHIndex(-m, l)] *= truncatedCosSh;
                K[getSHIndex(m, l)] *= truncatedCosSh;
            }
        }
    }

    // apply all the scaling factors
    for (unsigned int i = 0; i < numCoefficients; i++) {
        sh[i] *= K[i];
    }

    if (preprocessForShaders) {
        preprocessSHForShaders(sh);
    }

    return sh;
}

constexpr unsigned int SphericalHarmonics::getSHIndex(const int m, const unsigned int l) {
    return l * (l + 1) + m;
}

void SphericalHarmonics::calculateSHBasis(float* shB, const unsigned int bands, const glm::vec3& s) {
    /*
     * Below, we compute the associated Legendre polynomials using recursion.
     * see: http://mathworld.wolfram.com/AssociatedLegendrePolynomial.html
     *
     * Note [0]: s.z == cos(theta) ==> we only need to compute P(s.z)
     *
     * Note [1]: We in fact compute P(s.z) / sin(theta)^|m|, by removing
     * the "sqrt(1 - s.z*s.z)" [i.e.: sin(theta)] factor from the recursion.
     * This is later corrected in the ( cos(m*phi), sin(m*phi) ) recursion.
     */

    // s = (x, y, z) = (sin(theta)*cos(phi), sin(theta)*sin(phi), cos(theta))

    // handle m=0 separately, since it produces only one coefficient
    float Pml_2 = 0;
    float Pml_1 = 1;
    shB[0] = Pml_1;
    for (unsigned int l = 1; l < bands; l++) {
        const float Pml = ((2 * l - 1.0f) * Pml_1 * s.z - (l - 1.0f) * Pml_2) / l;
        Pml_2 = Pml_1;
        Pml_1 = Pml;
        shB[getSHIndex(0, l)] = Pml;
    }
    float Pmm = 1;
    for (unsigned int m = 1; m < bands; m++) {
        Pmm = (1.0f - 2 * m) * Pmm; // See [1], divide by sqrt(1 - s.z*s.z);
        Pml_2 = Pmm;
        Pml_1 = (2 * m + 1.0f) * Pmm * s.z;
        // l == m
        shB[getSHIndex(-m, m)] = Pml_2;
        shB[getSHIndex(m, m)] = Pml_2;
        if (m + 1 < bands) {
            // l == m+1
            shB[getSHIndex(-m, m + 1)] = Pml_1;
            shB[getSHIndex(m, m + 1)] = Pml_1;
            for (size_t l = m + 2; l < bands; l++) {
                const float Pml = ((2 * l - 1.0f) * Pml_1 * s.z - (l + m - 1.0f) * Pml_2) / (l - m);
                Pml_2 = Pml_1;
                Pml_1 = Pml;
                shB[getSHIndex(-m, l)] = Pml;
                shB[getSHIndex(m, l)] = Pml;
            }
        }
    }

    // At this point, SHb contains the associated Legendre polynomials divided
    // by sin(theta)^|m|. Below we compute the SH basis.
    //
    // ( cos(m*phi), sin(m*phi) ) recursion:
    // cos(m*phi + phi) == cos(m*phi)*cos(phi) - sin(m*phi)*sin(phi)
    // sin(m*phi + phi) == sin(m*phi)*cos(phi) + cos(m*phi)*sin(phi)
    // cos[m+1] == cos[m]*s.x - sin[m]*s.y
    // sin[m+1] == sin[m]*s.x + cos[m]*s.y
    //
    // Note that (d.x, d.y) == (cos(phi), sin(phi)) * sin(theta), so the
    // code below actually evaluates:
    //      (cos((m*phi), sin(m*phi)) * sin(theta)^|m|
    float Cm = s.x;
    float Sm = s.y;
    for (unsigned int m = 1; m <= bands; m++) {
        for (unsigned int l = m; l < bands; l++) {
            shB[getSHIndex(-m, l)] *= Sm;
            shB[getSHIndex(m, l)] *= Cm;
        }
        const float Cm1 = Cm * s.x - Sm * s.y;
        const float Sm1 = Sm * s.x + Cm * s.y;
        Cm = Cm1;
        Sm = Sm1;
    }
}

/*
 * This computes the 3-bands SH coefficients of the CubeMap convoluted by the truncated
 * cos(theta) (i.e.: saturate(s.z)), pre-scaled by the reconstruction factors.
 */
void SphericalHarmonics::preprocessSHForShaders(std::vector<glm::vec3>& sh) {
    // Coefficient for the polynomial form of the SH functions -- these were taken from
    // "Stupid Spherical Harmonics (SH)" by Peter-Pike Sloan
    // They simply come for expanding the computation of each SH function.
    //
    // To render spherical harmonics we can use the polynomial form, like this:
    //          c += sh[0] * A[0];
    //          c += sh[1] * A[1] * s.y;
    //          c += sh[2] * A[2] * s.z;
    //          c += sh[3] * A[3] * s.x;
    //          c += sh[4] * A[4] * s.y * s.x;
    //          c += sh[5] * A[5] * s.y * s.z;
    //          c += sh[6] * A[6] * (3 * s.z * s.z - 1);
    //          c += sh[7] * A[7] * s.z * s.x;
    //          c += sh[8] * A[8] * (s.x * s.x - s.y * s.y);
    //
    // To save math in the shader, we pre-multiply our SH coefficient by the A[i] factors.
    // Additionally, we include the lambertian diffuse BRDF 1/pi.

    const float sqrtPi = glm::root_pi<float>();
    const float sqrt3 = glm::root_three<float>();
    const float sqrt5 = glm::root_five<float>();
    constexpr float sqrt15 = 3.8729833462f;
    std::vector A = {
        1.0f / (2.0f * sqrtPi), // 0  0
        -sqrt3 / (2.0f * sqrtPi), // 1 -1
        sqrt3 / (2.0f * sqrtPi), // 1  0
        -sqrt3 / (2.0f * sqrtPi), // 1  1
        sqrt15 / (2.0f * sqrtPi), // 2 -2
        -sqrt15 / (2.0f * sqrtPi), // 3 -1
        sqrt5 / (4.0f * sqrtPi), // 3  0
        -sqrt15 / (2.0f * sqrtPi), // 3  1
        sqrt15 / (4.0f * sqrtPi) // 3  2
    };

    for (size_t i = 0; i < sh.size(); i++) {
        sh[i] *= A[i] * glm::one_over_pi<float>();
    }
}

// < cos(theta) > SH coefficients pre-multiplied by 1 / K(0,l)
constexpr float SphericalHarmonics::calculateTruncatedCosSH(const unsigned int l) {
    if (l == 0) {
        return glm::pi<float>();
    }
    if (l == 1) {
        return 2 * glm::pi<float>() / 3;
    }
    if (l & 1u) {
        return 0;
    }
    const size_t l_2 = l / 2;
    const float A0 = ((l_2 & 1u) ? 1.0f : -1.0f) / ((l + 2) * (l - 1));
    const float A1 = factorial(l, l_2) / (factorial(l_2) * (1 << l));
    return 2 * glm::pi<float>() * A0 * A1;
}

constexpr std::vector<float> SphericalHarmonics::calculateKi(const unsigned int bands) {
    const unsigned int numCoefficients = bands * bands;
    std::vector<float> K(numCoefficients);
    for (unsigned int l = 0; l < bands; l++) {
        K[getSHIndex(0, l)] = calculateKml(0, l);
        for (unsigned int m = 1; m <= l; m++) {
            K[getSHIndex(m, l)] = K[getSHIndex(-m, l)] = glm::root_two<float>() * calculateKml(m, l);
        }
    }
    return K;
}

/*
 * SH scaling factors:
 * returns sqrt((2*l + 1) / 4*pi) * sqrt( (l-|m|)! / (l+|m|)! )
 */
float SphericalHarmonics::calculateKml(int m, const unsigned int l) {
    // todo: m = m < 0 ? -m : m;
    m = std::abs(m);
    const float K = (2 * l + 1) * factorial(l - m, l + m);
    return std::sqrt(K) * (glm::two_over_root_pi<float>() * 0.25);
}

/*
 * returns n! / d!
 */
constexpr float SphericalHarmonics::factorial(unsigned int n, unsigned int d) {
    d = std::max(static_cast<unsigned int>(1), d);
    n = std::max(static_cast<unsigned int>(1), n);
    float r = 1.0;
    if (n == d) {
        return r;
    }

    if (n > d) {
        for (; n > d; n--) {
            r *= n;
        }
    } else {
        for (; d > n; d--) {
            r *= d;
        }
        r = 1.0f / r;
    }
    return r;
}

float SphericalHarmonics::sphereQuadrantArea(const float x, const float y) {
    return std::atan2(x * y, std::sqrt(x * x + y * y + 1));
}

float SphericalHarmonics::solidAngle(const unsigned int dim, const unsigned int u, const unsigned int v) {
    const float iDim = 1.0f / dim;
    const float s = ((u + 0.5f) * 2 * iDim) - 1;
    const float t = ((v + 0.5f) * 2 * iDim) - 1;
    const float x0 = s - iDim;
    const float y0 = t - iDim;
    const float x1 = s + iDim;
    const float y1 = t + iDim;
    const float solidAngle = sphereQuadrantArea(x0, y0) - sphereQuadrantArea(x0, y1) - sphereQuadrantArea(x1, y0) + sphereQuadrantArea(x1, y1);
    return solidAngle;
}
