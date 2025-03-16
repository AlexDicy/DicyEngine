#include "pch/enginepch.h"
#include "Plane.h"

#include "Model.h"

Ref<Mesh> Plane::create(const Ref<Renderer>& renderer, const Material& material) {
    const VertexData vertices[] = {
        VertexData(-1.0f, -1.0f, 0.0f, glm::vec3(0.0f), glm::vec2(0.0f, 0.0f)),
        VertexData(-1.0f, 1.0f, 0.0f, glm::vec3(0.0f), glm::vec2(0.0f, 1.0f)),
        VertexData(1.0f, 1.0f, 0.0f, glm::vec3(0.0f), glm::vec2(1.0f, 1.0f)),
        VertexData(1.0f, -1.0f, 0.0f, glm::vec3(0.0f), glm::vec2(1.0f, 0.0f)),
    };
    const unsigned int indexes[] = {0, 1, 2, 2, 3, 0};
    const auto vertexDataFloats = reinterpret_cast<const float*>(vertices);
    return std::make_shared<Mesh>(renderer, vertexDataFloats, sizeof(vertices), indexes, sizeof(indexes), material, glm::mat4(1.0f));
}
