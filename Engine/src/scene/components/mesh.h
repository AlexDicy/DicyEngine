#pragma once
#include "rendering/renderer.h"
#include "rendering/vertex_array.h"

struct Mesh {
    Ref<VertexArray> vertex_array;
    Ref<Texture2D> texture;

    Mesh(const Ref<Renderer>& renderer, const float* vertices, const size_t vertices_size, const uint32_t* indexes, const uint32_t indexes_count,
         const Ref<Texture2D>& texture = nullptr) {
        this->texture = texture;
        const Ref<VertexBuffer> vertex_buffer = renderer->create_vertex_buffer(vertices, static_cast<uint32_t>(vertices_size));

        vertex_buffer->set_layout({
            {DataType::FLOAT3, "position"},
            {DataType::FLOAT4, "color"},
            {DataType::FLOAT3, "normal"},
        });

        const Ref<IndexBuffer> index_buffer = renderer->create_index_buffer(indexes, indexes_count);
        vertex_array = renderer->create_vertex_array(vertex_buffer, index_buffer);
    }
};
