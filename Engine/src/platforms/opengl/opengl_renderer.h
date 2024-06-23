#pragma once
#include "rendering/renderer.h"

class OpenGLRenderer : public Renderer {
public:
    OpenGLRenderer() : Renderer(RenderAPI::OPENGL) {}

    VertexArray* create_vertex_array(const std::shared_ptr<VertexBuffer>& vertex_buffer, const std::shared_ptr<IndexBuffer>& index_buffer) const override;
    VertexBuffer* create_vertex_buffer(const float* vertices, uint32_t size) const override;
    IndexBuffer* create_index_buffer(const uint32_t* indexes, uint32_t count) const override;

    void clean() const override;
    void draw(const std::shared_ptr<VertexArray>& vertex_array) const override;
};
