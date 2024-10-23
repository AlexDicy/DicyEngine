#include "pch/enginepch.h"
#include "SkyboxCube.h"

SkyboxCube::SkyboxCube(const Ref<Renderer>& renderer, const Ref<Shader>& shader, const Ref<Texture>& texture) : shader(shader), texture(texture) {
    constexpr float vertices[3 * 8] = {
        -1.0f, -1.0f, -1.0f, //
        -1.0f, -1.0f, 1.0f, //
        -1.0f, 1.0f,  1.0f, //
        -1.0f, 1.0f,  -1.0f, //
        1.0f,  1.0f,  -1.0f, //
        1.0f,  -1.0f, -1.0f, //
        1.0f,  -1.0f, 1.0f, //
        1.0f,  1.0f,  1.0f, //
    };

    constexpr unsigned int indexes[36] = {
        0, 1, 2, 2, 3, 0, //
        4, 5, 6, 6, 7, 4, //
        3, 2, 7, 7, 4, 3, //
        0, 1, 6, 6, 5, 0, //
        1, 2, 7, 7, 6, 1, //
        0, 3, 4, 4, 5, 0, //
    };
    const Ref<VertexBuffer> vertexBuffer = renderer->createVertexBuffer(vertices, sizeof(vertices));
    vertexBuffer->setLayout({
        {DataType::FLOAT3, "position"},
    });
    this->vertexArray = renderer->createVertexArray(vertexBuffer, renderer->createIndexBuffer(indexes, 36));
}
