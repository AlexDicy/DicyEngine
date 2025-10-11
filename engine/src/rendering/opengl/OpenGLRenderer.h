#pragma once
#include "rendering/Renderer.h"
#include "rendering/Shader.h"

class OpenGLRenderer : public Renderer {
public:
    explicit OpenGLRenderer() : Renderer(RenderAPI::OPENGL) {}

    void init(unsigned int width, unsigned int height) override;

    void createRenderFramebuffer(unsigned int width, unsigned int height) override;
    void createRenderPassFramebuffers(unsigned int width, unsigned int height) override;
    void createDataFramebuffer(unsigned int width, unsigned int height) override;

    Ref<VertexArray> createVertexArray(const Ref<VertexBuffer>& vertexBuffer, const Ref<IndexBuffer>& indexBuffer) const override;
    Ref<VertexBuffer> createVertexBuffer(const float* vertices, unsigned int size) const override;
    Ref<IndexBuffer> createIndexBuffer(const unsigned int* indexes, unsigned int count) const override;
    Ref<Shader> createShader(const std::string& vertexPath, const std::string& fragmentPath) const override;
    Ref<Texture> createTexture(const Texture::TextureParams& params, const void* data = nullptr) const override;
    Ref<Texture> createBRDFLUT(const Ref<Shader>& shader, unsigned int size) const override;
    Ref<Texture> createTextureCubeFromHDR(const Ref<Texture>& hdrTexture, const Ref<Shader>& convertShader, unsigned int size) override;
    Ref<Texture> createPrefilteredCubemap(const Ref<Texture>& textureCube, const Ref<Shader>& convertShader, unsigned int size) override;

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
};
