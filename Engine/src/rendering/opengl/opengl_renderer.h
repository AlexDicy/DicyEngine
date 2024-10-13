#pragma once
#include "opengl_texture_2d.h"
#include "rendering/renderer.h"
#include "rendering/shader.h"

class OpenGLRenderer : public Renderer, public std::enable_shared_from_this<OpenGLRenderer> {
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
    Ref<Texture2D> create_texture2d(unsigned int channels, unsigned int width, unsigned int height, const void* data) const override;
    Ref<TextureCube> create_texture_cube(const std::array<std::string, 6>& paths) const override;
    Ref<TextureCube> create_texture_cube_from_hdr(const Ref<Texture2D>& hdr_texture, const Ref<Shader>& convert_shader, uint32_t size) override;

    void begin_frame() override;
    void end_frame() const override;
    void clean() const override;
    void add_point_light(const PointLight& point_light) override;
    void draw(const Ref<VertexArray>& vertex_array, const Ref<Shader>& shader, const glm::mat4& transform, const Ref<DirectionalLight>& directional_light) const override;
    void draw(const Ref<VertexArray>& vertex_array, const Ref<Shader>& shader, const glm::mat4& transform, const Ref<DirectionalLight>& directional_light,
              const Material& material) const override;
    void draw_skybox(const Ref<SkyboxCube>& skybox) const override;

private:
    Ref<Framebuffer> framebuffer;
    Ref<OpenGLTexture2D> white_pixel_texture;
    Ref<OpenGLTexture2D> default_occlusion_roughness_metallic_texture;
};
