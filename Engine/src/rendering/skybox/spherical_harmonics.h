#pragma once
#include "images/CubeMap.h"


class SphericalHarmonics {
public:
    SphericalHarmonics() = default;
    virtual ~SphericalHarmonics() = default;

    struct Sample {
        CubeMap::Face cube_map_side;
        uint64_t x;
        uint64_t y;
        double coefficients[9];
    };


private:
    static void calculateSHBasis(float* shB, uint64_t bands, const glm::vec3& s);
    static constexpr float calculateTruncatedCosSH(uint64_t l);
    static constexpr std::vector<float> calculateKi(uint64_t bands);
    static float calculateKml(int64_t m, uint64_t l);
    static constexpr float factorial(uint64_t n, uint64_t d = 1);
};
