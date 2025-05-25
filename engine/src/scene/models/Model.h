#pragma once
#include "scene/materials/Material.h"

struct VertexData {
    float x, y, z;
    glm::vec3 normal;
    glm::vec2 textureCoords;

    VertexData(const float x, const float y, const float z, const glm::vec3 normal, const glm::vec2 textureCoords) //
        : x(x), y(y), z(z), normal(normal), textureCoords(textureCoords) {}
};

struct Model {
    std::string path;
    unsigned int meshIndex;

    std::vector<VertexData> vertices;
    std::vector<unsigned int> indexes;
    glm::mat4 transformationMatrix;
    Material material;

    explicit Model(std::string path, const unsigned int meshIndex = 0) : path(std::move(path)), meshIndex(meshIndex), transformationMatrix() {}
};
