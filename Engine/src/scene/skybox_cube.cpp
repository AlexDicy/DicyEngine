#include "pch/enginepch.h"
#include "skybox_cube.h"

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
    const Ref<VertexBuffer> vertex_buffer = renderer->create_vertex_buffer(vertices, sizeof(vertices));
    vertex_buffer->set_layout({
        {DataType::FLOAT3, "position"},
    });
    this->vertex_array = renderer->create_vertex_array(vertex_buffer, renderer->create_index_buffer(indexes, 36));
}
