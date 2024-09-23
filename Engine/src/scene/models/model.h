#pragma once

struct VertexData {
    float x, y, z;
    float r, g, b, a;
};

struct Model {
    std::vector<VertexData> vertices;
    std::vector<unsigned int> indexes;
};
