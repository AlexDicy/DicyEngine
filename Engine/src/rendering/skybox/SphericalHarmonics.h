#pragma once
#include "images/CubeMap.h"


class SphericalHarmonics {
public:
    static std::vector<glm::vec3> calculateSH(const Ref<CubeMap>& cubeMap, unsigned int bands, bool calculateForIrradiance, bool preprocessForShaders);
    static void preprocessSHForShaders(std::vector<glm::vec3>& sh);

private:
    static constexpr unsigned int getSHIndex(int m, unsigned int l);
    static void calculateSHBasis(float* shB, unsigned int bands, const glm::vec3& s);
    static constexpr float calculateTruncatedCosSH(unsigned int l);
    static constexpr std::vector<float> calculateKi(unsigned int bands);
    static float calculateKml(int m, unsigned int l);
    static constexpr float factorial(unsigned int n, unsigned int d = 1);
    static float sphereQuadrantArea(float x, float y);
    static float solidAngle(unsigned int dim, unsigned int u, unsigned int v);
};
