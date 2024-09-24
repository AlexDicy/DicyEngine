#pragma once
#include "opengl_texture.h"
#include "rendering/renderer.h"
#include "rendering/shader.h"

class OpenGLRenderer : public Renderer {
public:
    OpenGLRenderer() : Renderer(RenderAPI::OPENGL) {}

    void init(int x, int y, uint32_t width, uint32_t height) override;
    void set_viewport(int x, int y, uint32_t width, uint32_t height) override;
    const Ref<Framebuffer>& get_framebuffer() const override;

    Ref<VertexArray> create_vertex_array(const Ref<VertexBuffer>& vertex_buffer, const Ref<IndexBuffer>& index_buffer) const override;
    Ref<VertexBuffer> create_vertex_buffer(const float* vertices, uint32_t size) const override;
    Ref<IndexBuffer> create_index_buffer(const uint32_t* indexes, uint32_t count) const override;
    Ref<Shader> create_shader(const std::string& vertex_path, const std::string& fragment_path) const override;
    Ref<Texture2D> create_texture2d(const std::string& path) const override;

    void begin_frame() override;
    void end_frame() const override;
    void clean() const override;
    void draw(const Ref<VertexArray>& vertex_array, const Ref<Shader>& shader, const glm::mat4& transform) const override;
    void draw(const Ref<VertexArray>& vertex_array, const Ref<Shader>& shader, const glm::mat4& transform, const Ref<Texture>& texture) const override;

private:
    Ref<Framebuffer> framebuffer;
    Ref<OpenGLTexture2D> white_pixel_texture;
};
