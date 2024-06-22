#pragma once
#include "buffer.h"
#include "vertex_array.h"

enum class RenderAPI { NONE, OPENGL };

class Renderer {
public:
    Renderer();

    VertexArray* create_vertex_array(const std::shared_ptr<VertexBuffer>& vertex_buffer, const std::shared_ptr<IndexBuffer>& index_buffer) const;
    VertexBuffer* create_vertex_buffer(const float* vertices, uint32_t size) const;
    IndexBuffer* create_index_buffer(const uint32_t* indexes, uint32_t count) const;

private:
    RenderAPI api;
};
