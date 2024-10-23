#pragma once
#include "opengl_texture_2d.h"
#include "rendering/renderer.h"
#include "rendering/shader.h"

class OpenGLRenderer : public Renderer, public std::enable_shared_from_this<OpenGLRenderer> {
public:
    OpenGLRenderer() : Renderer(RenderAPI::OPENGL) {}

    void init(int x, int y, uint32_t width, uint32_t height) override;
    void setViewport(int x, int y, uint32_t width, uint32_t height) override;
    const Ref<Framebuffer>& getFramebuffer() const override;

    Ref<VertexArray> createVertexArray(const Ref<VertexBuffer>& vertexBuffer, const Ref<IndexBuffer>& indexBuffer) const override;
    Ref<VertexBuffer> createVertexBuffer(const float* vertices, uint32_t size) const override;
    Ref<IndexBuffer> createIndexBuffer(const uint32_t* indexes, uint32_t count) const override;
    Ref<Shader> createShader(const std::string& vertexPath, const std::string& fragmentPath) const override;
    Ref<Texture2D> createTexture2D(const std::string& path) const override;
    Ref<Texture2D> createTexture2D(unsigned int channels, unsigned int width, unsigned int height, unsigned int bytesPerPixel, const void* data) const override;
    Ref<TextureCube> createTextureCube(const std::array<std::string, 6>& paths) const override;
    Ref<TextureCube> createTextureCubeFromHDR(const Ref<Texture2D>& hdrTexture, const Ref<Shader>& convertShader, uint32_t size) override;
    Ref<TextureCube> createIrradianceMap(const Ref<TextureCube>& textureCube, const Ref<Shader>& irradianceShader, uint32_t size) override;

    void beginFrame() override;
    void endFrame() const override;
    void clean() const override;
    void addPointLight(const PointLight& pointLight) override;
    void draw(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const glm::mat4& transform, const Ref<DirectionalLight>& directionalLight) const override;
    void draw(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const glm::mat4& transform, const Ref<DirectionalLight>& directionalLight,
              const Material& material) const override;
    void drawSkybox(const Ref<SkyboxCube>& skybox) const override;

private:
    Ref<Framebuffer> framebuffer;
    Ref<OpenGLTexture2D> whitePixelTexture;
    Ref<OpenGLTexture2D> defaultOcclusionRoughnessMetallicTexture;
};
