#include "pch/enginepch.h"
#include "renderer.h"

#include "platforms/opengl/opengl_buffer.h"

Renderer::Renderer() {
    this->api = RenderAPI::OPENGL;
}


VertexBuffer* Renderer::create_vertex_buffer(const float* vertices, const uint32_t size) const {
    switch (this->api) {
        case RenderAPI::OPENGL:
            return new OpenGLVertexBuffer(vertices, size);
        case RenderAPI::NONE:
            break;
    }
    return nullptr;
}

IndexBuffer* Renderer::create_index_buffer(const uint32_t* indexes, const uint32_t count) const {
    switch (api) {
        case RenderAPI::OPENGL:
            return new OpenGLIndexBuffer(indexes, count);
        case RenderAPI::NONE:
            break;
    }
    return nullptr;
}
