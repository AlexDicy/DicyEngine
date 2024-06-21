#pragma once
#include "buffer.h"

enum RenderAPI { NONE, OPENGL };

class Renderer {
public:
    Renderer();
    Buffer* create_vertex_buffer(const float* vertices, uint32_t size) const;
    Buffer* create_index_buffer(const uint32_t* indexes, uint32_t count) const;

private:
    RenderAPI api;
};
