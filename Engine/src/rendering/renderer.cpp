#include "pch/enginepch.h"
#include "renderer.h"

#include "platforms/opengl/opengl_buffer.h"

Renderer::Renderer() {
    this->api = OPENGL;
}


Buffer* Renderer::create_vertex_buffer(const float* vertices, const uint32_t size) const {
    switch (this->api) {
        case OPENGL:
            return new OpenGLVertexBuffer(vertices, size);
        case NONE:
            break;
    }
    return nullptr;
}

Buffer* Renderer::create_index_buffer(const uint32_t* indexes, const uint32_t count) const {
    switch (api) {
        case OPENGL:
            return new OpenGLIndexBuffer(indexes, count);
        case NONE:
            break;
    }
    return nullptr;
}
