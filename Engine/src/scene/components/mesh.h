#pragma once
#include "rendering/renderer.h"
#include "rendering/vertex_array.h"
#include "scene/materials/material.h"

struct Mesh {
    Ref<VertexArray> vertex_array;
    Material material;
    glm::mat4 transformation_matrix;

    Mesh(const Ref<Renderer>& renderer, const float* vertices, const size_t vertices_size, const uint32_t* indexes, const uint32_t indexes_count, const Material& material, const glm::mat4& transformation_matrix = glm::mat4(1.0f)) {
        this->material = material;
        this->transformation_matrix = transformation_matrix;

        const Ref<VertexBuffer> vertex_buffer = renderer->create_vertex_buffer(vertices, static_cast<uint32_t>(vertices_size));
        vertex_buffer->set_layout({
            {DataType::FLOAT3, "position"},
            {DataType::FLOAT3, "normal"},
            {DataType::FLOAT2, "texture_coords"},
        });

        const Ref<IndexBuffer> index_buffer = renderer->create_index_buffer(indexes, indexes_count);
        vertex_array = renderer->create_vertex_array(vertex_buffer, index_buffer);
    }
};
