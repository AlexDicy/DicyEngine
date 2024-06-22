#include "pch/enginepch.h"
#include "renderer.h"

#include "platforms/opengl/opengl_buffer.h"
#include "platforms/opengl/opengl_vertex_array.h"

Renderer::Renderer() {
    this->api = RenderAPI::OPENGL;
}


VertexArray* Renderer::create_vertex_array(const std::shared_ptr<VertexBuffer>& vertex_buffer, const std::shared_ptr<IndexBuffer>& index_buffer) const {
    switch (this->api) {
        case RenderAPI::OPENGL:
            return new OpenGLVertexArray(vertex_buffer, index_buffer);
        case RenderAPI::NONE:
            break;
    }
    return nullptr;
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
