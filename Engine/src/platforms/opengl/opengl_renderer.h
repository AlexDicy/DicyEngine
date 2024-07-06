#pragma once
#include "rendering/renderer.h"
#include "rendering/shader.h"

class OpenGLRenderer : public Renderer {
public:
    OpenGLRenderer() : Renderer(RenderAPI::OPENGL) {}

    void set_viewport(int x, int y, uint32_t width, uint32_t height) override;

    VertexArray* create_vertex_array(const Ref<VertexBuffer>& vertex_buffer, const Ref<IndexBuffer>& index_buffer) const override;
    VertexBuffer* create_vertex_buffer(const float* vertices, uint32_t size) const override;
    IndexBuffer* create_index_buffer(const uint32_t* indexes, uint32_t count) const override;

    void begin_frame() override;
    void end_frame() const override;
    void clean() const override;
    void draw(const Ref<VertexArray>& vertex_array, const Ref<Shader>& shader, const glm::mat4& transform) const override;
};
