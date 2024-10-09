#pragma once
#include "scene/materials/material.h"

struct VertexData {
    float x, y, z;
    glm::vec3 normal;
    glm::vec2 texture_coords;

    VertexData(const float x, const float y, const float z, const glm::vec3 normal, const glm::vec2 texture_coords) //
        : x(x), y(y), z(z), normal(normal), texture_coords(texture_coords) {}
};

struct Model {
    std::vector<VertexData> vertices;
    std::vector<unsigned int> indexes;
    glm::mat4 transformation_matrix;
    Material material;
};
