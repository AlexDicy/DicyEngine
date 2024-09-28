#pragma once
#include "scene/materials/material.h"

struct VertexData {
    float x, y, z;
    float r, g, b, a;
    glm::vec3 normal;
};

struct Model {
    std::vector<VertexData> vertices;
    std::vector<unsigned int> indexes;
    Material material;
    Ref<Texture2D> texture;
};
