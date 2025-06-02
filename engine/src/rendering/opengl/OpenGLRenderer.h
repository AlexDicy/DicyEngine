#pragma once
#include "OpenGLTexture2D.h"
#include "framebuffer/OpenGLRenderFramebuffer.h"
#include "rendering/Renderer.h"
#include "rendering/Shader.h"

class OpenGLRenderer : public Renderer, public std::enable_shared_from_this<OpenGLRenderer> {
public:
    OpenGLRenderer() : Renderer(RenderAPI::OPENGL) {}

    void init(uint32_t width, uint32_t height) override;
    void setFramebufferDimensions(unsigned int width, unsigned int height) override;
    Ref<RenderFramebuffer> getFramebuffer() const override;

    Ref<VertexArray> createVertexArray(const Ref<VertexBuffer>& vertexBuffer, const Ref<IndexBuffer>& indexBuffer) const override;
    Ref<VertexBuffer> createVertexBuffer(const float* vertices, uint32_t size) const override;
    Ref<IndexBuffer> createIndexBuffer(const uint32_t* indexes, uint32_t count) const override;
    Ref<Shader> createShader(const std::string& vertexPath, const std::string& fragmentPath) const override;
    Ref<Texture2D> createTexture2D(const std::string& path) const override;
    Ref<Texture2D> createTexture2D(unsigned int channels, unsigned int width, unsigned int height, unsigned int bytesPerPixel, const void* data) const override;
    Ref<Texture2D> createTexture2D(unsigned int channels, unsigned int width, unsigned int height, unsigned int bytesPerPixel, TextureFormat format,
                                   const void* data) const override;
    Ref<Texture2D> createBRDFLUT(const Ref<Shader>& shader, uint32_t size) const override;
    Ref<TextureCube> createTextureCube(const std::array<std::string, 6>& paths) const override;
    Ref<TextureCube> createTextureCubeFromHDR(const Ref<Texture2D>& hdrTexture, const Ref<Shader>& convertShader, uint32_t size) override;
    Ref<TextureCube> createPrefilteredCubemap(const Ref<TextureCube>& textureCube, const Ref<Shader>& convertShader, uint32_t size) override;

    void beginFrame() override;
    void beginDirectionalShadows() const override;
    void beginPointLightShadows() const override;
    void beginPointLightShadow(const PointLight& light, int lightIndex, int faceIndex) const override;
    void endShadows() const override;
    void endFrame() const override;
    void clear() const override;
    void drawToMainFramebuffer() const override;

    void draw(unsigned int entityId, const Ref<VertexArray>& vertexArray, const glm::mat4& transform, const Ref<Shader>& shader) const override;
    void draw(unsigned int entityId, const Ref<VertexArray>& vertexArray, const glm::mat4& transform, const Ref<Shader>& shader, const Material& material) const override;
    void drawForDirectionalShadows(const Ref<VertexArray>& vertexArray, const glm::mat4& transform) const override;
    void drawForPointLightShadows(const Ref<VertexArray>& vertexArray, const glm::mat4& transform) const override;
    void drawJumpFloodingPrepare(const Ref<VertexArray>& vertexArray, const glm::mat4& transform, const Ref<Shader>& outlineShader) const override;
    void drawSkybox(const Ref<SkyboxCube>& skybox) const override;
    void drawJumpFloodingPass(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, int offset, bool vertical) override;
    void drawEditorOverlays(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, glm::vec4 outlineColor, float outlineWidth) const override;
    void drawUI(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const Material& material) const override;

private:
    Ref<OpenGLRenderFramebuffer> framebuffer;
    Ref<OpenGLTexture2D> whitePixelTexture;
    Ref<OpenGLTexture2D> defaultOcclusionRoughnessMetallicTexture;
};
