#pragma once

//
// https://metashapes.com/blog/realtime-image-based-lighting-using-spherical-harmonics/
//

class SphericalHarmonics {
public:
    SphericalHarmonics() = default;
    virtual ~SphericalHarmonics() = default;

    enum class CubeMapFace : uint8_t {
        RIGHT = 0, //             +----+
        LEFT = 1, //              | 2  |
        TOP = 2, //          +----+----+----+----+
        BOTTOM = 3, //       | 1  | 4  | 0  | 5  |
        FRONT = 4, //        +----+----+----+----+
        BACK = 5, //              | 3  |
        //                        +----+
    };

    struct Sample {
        CubeMapFace cube_map_side;
        uint64_t x;
        uint64_t y;
        double coefficients[9];
    };

    static Ref<SphericalHarmonics> calculate_sh_for_cubemap(uint64_t resolution_x, uint64_t resolution_y);

private:
    static void calculateSHBasis(float* shB, uint64_t numBands, const glm::vec3& s);
    static constexpr float SphericalHarmonics::calculateTruncatedCosSH(uint64_t l);
    static constexpr std::vector<float> SphericalHarmonics::calculateKi(uint64_t numBands);
    static constexpr float calculateKml(int64_t m, uint64_t l);
    static constexpr float factorial(uint64_t n, uint64_t d);
};
