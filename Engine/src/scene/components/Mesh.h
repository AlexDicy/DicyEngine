#pragma once
#include "rendering/Renderer.h"
#include "rendering/VertexArray.h"
#include "scene/materials/Material.h"

struct Mesh {
    Ref<VertexArray> vertexArray;
    Material material;
    glm::mat4 transformationMatrix;

    Mesh(const Ref<Renderer>& renderer, const float* vertices, const size_t verticesSize, const uint32_t* indexes, const uint32_t indexesCount, const Material& material, const glm::mat4& transformationMatrix = glm::mat4(1.0f)) {
        this->material = material;
        this->transformationMatrix = transformationMatrix;

        const Ref<VertexBuffer> vertexBuffer = renderer->createVertexBuffer(vertices, static_cast<uint32_t>(verticesSize));
        vertexBuffer->setLayout({
            {DataType::FLOAT3, "position"},
            {DataType::FLOAT3, "normal"},
            {DataType::FLOAT2, "texture_coords"},
        });

        const Ref<IndexBuffer> indexBuffer = renderer->createIndexBuffer(indexes, indexesCount);
        vertexArray = renderer->createVertexArray(vertexBuffer, indexBuffer);
    }
};
