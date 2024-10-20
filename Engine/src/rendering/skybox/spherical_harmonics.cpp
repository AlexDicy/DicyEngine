#include "pch/enginepch.h"
#include "spherical_harmonics.h"

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyClangDiagnosticImplicitIntFloatConversion
// ReSharper disable CppClangTidyBugproneNarrowingConversions
// ReSharper disable CppClangTidyClangDiagnosticImplicitFloatConversion

//
// Heavily inspired by Google Filament's implementation
//

static constexpr uint64_t getSHIndex(const int64_t m, const uint64_t l) {
    return l * (l + 1) + m;
}

/*
std::vector<glm::vec3> SphericalHarmonics::calculateSH(const Cubemap& cm, uint64_t numBands, const bool irradiance) {
    const uint64_t numCoefficients = numBands * numBands;
    std::vector<glm::vec3> SH(numCoefficients);

    for (uint64_t side = 0; side < 6; side++) {
        std::vector<glm::vec3> currentSH(numCoefficients);
        std::vector<float> currentSHb(numCoefficients);

        for (uint64_t i = 0; i < numCoefficients; i++) {
            SH[i] += currentSH[i];
        }
    }

    struct State {
        State() = default;
        explicit State(size_t numCoefs) : numCoefficients(numCoefs) {}

        State& operator=(State const& rhs) {
            SH.reset(new glm::vec3[rhs.numCoefficients]{});
            SHb.reset(new float[rhs.numCoefficients]{});
            return *this;
        }
        size_t numCoefficients = 0;
        std::unique_ptr<glm::vec3[]> SH;
        std::unique_ptr<float[]> SHb;
    } prototype(numCoefficients);

    CubemapUtils::process<State>(
        const_cast<Cubemap&>(cm), js,
        [&](State& state, size_t y, CubeMapFace face, Cubemap::Texel const* data, size_t dim) {
            for (size_t x = 0; x < dim; ++x, ++data) {

                glm::vec3 s(cm.getDirectionFor(face, x, y));

                // sample a color
                glm::vec3 color(Cubemap::sampleAt(data));

                // take solid angle into account
                color *= CubemapUtils::solidAngle(dim, x, y);

                calculateSHBasis(state.SHb.get(), numBands, s);

                // apply coefficients to the sampled color
                for (size_t i = 0; i < numCoefficients; i++) {
                    state.SH[i] += color * state.SHb[i];
                }
            }
        },
        prototype);

    // precompute the scaling factor K
    std::vector<float> K = calculateKi(numBands);

    // apply truncated cos (irradiance)
    if (irradiance) {
        for (uint64_t l = 0; l < numBands; l++) {
            const float truncatedCosSh = calculateTruncatedCosSH(l);
            K[getSHIndex(0, l)] *= truncatedCosSh;
            for (uint64_t m = 1; m <= l; m++) {
                K[getSHIndex(-m, l)] *= truncatedCosSh;
                K[getSHIndex(m, l)] *= truncatedCosSh;
            }
        }
    }

    // apply all the scale factors
    for (size_t i = 0; i < numCoefficients; i++) {
        SH[i] *= K[i];
    }
    return SH;
}*/

void SphericalHarmonics::calculateSHBasis(float* shB, const uint64_t bands, const glm::vec3& s) {
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
    for (uint64_t l = 1; l < bands; l++) {
        const float Pml = ((2 * l - 1.0f) * Pml_1 * s.z - (l - 1.0f) * Pml_2) / l;
        Pml_2 = Pml_1;
        Pml_1 = Pml;
        shB[getSHIndex(0, l)] = Pml;
    }
    float Pmm = 1;
    for (uint64_t m = 1; m < bands; m++) {
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
    for (uint64_t m = 1; m <= bands; m++) {
        for (uint64_t l = m; l < bands; l++) {
            shB[getSHIndex(-m, l)] *= Sm;
            shB[getSHIndex(m, l)] *= Cm;
        }
        const float Cm1 = Cm * s.x - Sm * s.y;
        const float Sm1 = Sm * s.x + Cm * s.y;
        Cm = Cm1;
        Sm = Sm1;
    }
}

// < cos(theta) > SH coefficients pre-multiplied by 1 / K(0,l)
constexpr float SphericalHarmonics::calculateTruncatedCosSH(const uint64_t l) {
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

constexpr std::vector<float> SphericalHarmonics::calculateKi(const uint64_t bands) {
    const uint64_t numCoefficients = bands * bands;
    std::vector<float> K(numCoefficients);
    for (uint64_t l = 0; l < bands; l++) {
        K[getSHIndex(0, l)] = calculateKml(0, l);
        for (uint64_t m = 1; m <= l; m++) {
            K[getSHIndex(m, l)] = K[getSHIndex(-m, l)] = glm::root_two<float>() * calculateKml(m, l);
        }
    }
    return K;
}

/*
 * SH scaling factors:
 * returns sqrt((2*l + 1) / 4*pi) * sqrt( (l-|m|)! / (l+|m|)! )
 */
float SphericalHarmonics::calculateKml(int64_t m, const uint64_t l) {
    //todo: m = m < 0 ? -m : m;
    m = std::abs(m);
    const float K = (2 * l + 1) * factorial(l - m, l + m);
    return std::sqrt(K) * (glm::two_over_root_pi<float>() * 0.25);
}

/*
 * returns n! / d!
 */
constexpr float SphericalHarmonics::factorial(uint64_t n, uint64_t d) {
    d = std::max(static_cast<uint64_t>(1), d);
    n = std::max(static_cast<uint64_t>(1), n);
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
