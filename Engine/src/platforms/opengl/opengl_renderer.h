#pragma once
#include "rendering/renderer.h"
#include "rendering/shader.h"

class OpenGLRenderer : public Renderer {
public:
    OpenGLRenderer() : Renderer(RenderAPI::OPENGL) {}

    void set_viewport(int x, int y, uint32_t width, uint32_t height) override;

    VertexArray* create_vertex_array(const std::shared_ptr<VertexBuffer>& vertex_buffer, const std::shared_ptr<IndexBuffer>& index_buffer) const override;
    VertexBuffer* create_vertex_buffer(const float* vertices, uint32_t size) const override;
    IndexBuffer* create_index_buffer(const uint32_t* indexes, uint32_t count) const override;

    void begin_frame(Camera& camera) override;
    void end_frame() const override;
    void clean() const override;
    void draw(const std::shared_ptr<VertexArray>& vertex_array, const std::shared_ptr<Shader>& shader, const glm::mat4& transform) const override;
};
