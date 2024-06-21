#pragma once
#include "buffer.h"

enum class RenderAPI { NONE, OPENGL };

class Renderer {
public:
    Renderer();
    VertexBuffer* create_vertex_buffer(const float* vertices, uint32_t size) const;
    IndexBuffer* create_index_buffer(const uint32_t* indexes, uint32_t count) const;

private:
    RenderAPI api;
};
