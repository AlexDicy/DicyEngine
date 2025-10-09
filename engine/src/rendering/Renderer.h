#pragma once
#include "camera/Camera.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include "framebuffer/DataFramebuffer.h"
#include "framebuffer/DepthFramebuffer.h"
#include "framebuffer/RenderFramebuffer.h"
#include "framebuffer/RenderPassFramebuffer.h"
#include "framebuffer/ShadowCubeArrayFramebuffer.h"
#include "scene/components/PointLight.h"
#include "scene/lights/DirectionalLight.h"
#include "scene/materials/Material.h"
#include "skybox/SkyboxCube.h"

enum class RenderAPI { NONE, OPENGL, VULKAN };

class Renderer {
public:
    explicit Renderer(const RenderAPI api) : api(api) {}
    virtual ~Renderer() = default;

    RenderAPI getAPI() const {
        return this->api;
    }

    struct Viewport {
        int x = 0;
        int y = 0;
        int width = 0;
        int height = 0;
    };

    const Ref<Camera>& getCamera() {
        return this->camera;
    }

    void setCamera(const Ref<Camera>& camera) {
        this->camera = camera;
    }

    virtual void init(uint32_t width, uint32_t height);

    void setFramebufferDimensions(unsigned int width, unsigned int height);
    virtual void createRenderFramebuffer(unsigned int width, unsigned int height) = 0;
    virtual void createRenderPassFramebuffers(unsigned int width, unsigned int height) = 0;
    virtual void createDataFramebuffer(unsigned int width, unsigned int height) = 0;

    void setViewport(int x, int y, uint32_t width, uint32_t height);

    const Viewport& getViewport() const {
        return this->viewport;
    }

    Ref<RenderFramebuffer> getFramebuffer() const {
        return this->framebuffer;
    }

    const Ref<DepthFramebuffer>& getShadowDepthFramebuffer() const;
    void swapPassFramebuffers();

    virtual Ref<VertexArray> createVertexArray(const Ref<VertexBuffer>& vertexBuffer, const Ref<IndexBuffer>& indexBuffer) const = 0;
    virtual Ref<VertexBuffer> createVertexBuffer(const float* vertices, uint32_t size) const = 0;
    virtual Ref<IndexBuffer> createIndexBuffer(const uint32_t* indexes, uint32_t count) const = 0;
    virtual Ref<Shader> createShader(const std::string& vertexPath, const std::string& fragmentPath) const = 0;
    virtual Ref<Texture> createTexture(unsigned int width, unsigned int height, unsigned int layers, Texture::Format format, Texture::InternalFormat internalFormat,
                                       Texture::TextureType type = Texture::TextureType::TEXTURE_2D, const void* data = nullptr) const = 0;
    virtual Ref<Texture> createBRDFLUT(const Ref<Shader>& shader, uint32_t width) const = 0;
    Ref<Texture> createTextureCube(const std::array<std::string, 6>& paths) const;
    virtual Ref<Texture> createTextureCubeFromHDR(const Ref<Texture>& hdrTexture, const Ref<Shader>& convertShader, uint32_t size) = 0;
    virtual Ref<Texture> createPrefilteredCubemap(const Ref<Texture>& textureCube, const Ref<Shader>& convertShader, uint32_t size) = 0;

    virtual void beginFrame() = 0;
    virtual void beginDirectionalShadows() const = 0;
    virtual void beginPointLightShadows() const = 0;
    virtual void beginPointLightShadow(const PointLight& light, int lightIndex, int faceIndex) const = 0;
    virtual void endShadows() const = 0;
    void endMeshes() const;
    virtual void endFrame() const = 0;
    virtual void clear() const = 0;
    virtual void drawToMainFramebuffer() const = 0;

    void setIrradianceSH(const std::array<glm::vec3, 9>& irradianceSh);
    void setPrefilteredEnvMap(const Ref<Texture>& prefilteredEnvMap);
    void setBRDFLUT(const Ref<Texture>& brdfLUT);
    // needs to be called before each frame
    void setDirectionalLight(const Ref<DirectionalLight>& directionalLight);
    void addPointLight(const PointLight& pointLight);
    void setDirectionalShadowMapShader(const Ref<Shader>& shadowMapShader);
    void setPointLightShadowMapShader(const Ref<Shader>& shader);

    virtual void draw(unsigned int entityId, const Ref<VertexArray>& vertexArray, const glm::mat4& transform, const Ref<Shader>& shader) const = 0;
    virtual void draw(unsigned int entityId, const Ref<VertexArray>& vertexArray, const glm::mat4& transform, const Ref<Shader>& shader, const Material& material) const = 0;
    virtual void drawForDirectionalShadows(const Ref<VertexArray>& vertexArray, const glm::mat4& transform) const = 0;
    virtual void drawForPointLightShadows(const Ref<VertexArray>& vertexArray, const glm::mat4& transform) const = 0;
    virtual void drawJumpFloodingPrepare(const Ref<VertexArray>& vertexArray, const glm::mat4& transform, const Ref<Shader>& outlineShader) const = 0;
    virtual void drawSkybox(const Ref<SkyboxCube>& skybox) const = 0;
    virtual void drawJumpFloodingPass(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, int offset, bool vertical) = 0;
    virtual void drawEditorOverlays(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, glm::vec4 outlineColor, float outlineWidth) const = 0;
    virtual void drawUI(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const Material& material) const = 0;

protected:
    Ref<Camera> camera;
    glm::mat4 viewProjectionMatrix = glm::identity<glm::mat4>();
    glm::mat4 viewMatrix = glm::identity<glm::mat4>();
    glm::mat4 projectionMatrix = glm::identity<glm::mat4>();
    Viewport viewport;

    Ref<RenderFramebuffer> framebuffer;
    Ref<RenderPassFramebuffer> previousPassFramebuffer; // used to reference in the current pass
    Ref<RenderPassFramebuffer> currentPassFramebuffer; // will be swapped with the previous one after each pass
    Ref<DataFramebuffer> dataFramebuffer;

    // default textures
    Ref<Texture> whitePixelTexture;
    Ref<Texture> defaultOcclusionRoughnessMetallicTexture;

    // lighting
    std::array<glm::vec3, 9> irradianceSH = std::array<glm::vec3, 9>();
    Ref<Texture> prefilteredEnvMap;
    Ref<Texture> brdfLUT;
    Ref<DirectionalLight> directionalLight;
    glm::mat4 directionalLightViewProjection = glm::identity<glm::mat4>();
    std::vector<PointLight> pointLights = std::vector<PointLight>();

    // shadow mapping
    Ref<DepthFramebuffer> shadowDepthFramebuffer;
    Ref<Shader> shadowMapShader;
    Ref<ShadowCubeArrayFramebuffer> shadowCubeArrayFramebuffer;
    Ref<Shader> shadowCubeArrayShader;

private:
    RenderAPI api;
};
