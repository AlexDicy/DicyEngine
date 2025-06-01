#pragma once
#include "camera/Camera.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include "framebuffer/DataFramebuffer.h"
#include "framebuffer/DepthFramebuffer.h"
#include "framebuffer/RenderFramebuffer.h"
#include "framebuffer/ShadowCubeArrayFramebuffer.h"
#include "scene/components/PointLight.h"
#include "scene/lights/DirectionalLight.h"
#include "scene/materials/Material.h"
#include "skybox/SkyboxCube.h"

enum class RenderAPI { NONE, OPENGL };

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

    virtual void init(uint32_t width, uint32_t height) = 0;
    virtual void setFramebufferDimensions(unsigned int width, unsigned int height) = 0;
    void setViewport(int x, int y, uint32_t width, uint32_t height);

    const Viewport& getViewport() const {
        return this->viewport;
    }

    virtual Ref<RenderFramebuffer> getFramebuffer() const = 0;
    const Ref<DepthFramebuffer>& getShadowDepthFramebuffer() const;

    virtual Ref<VertexArray> createVertexArray(const Ref<VertexBuffer>& vertexBuffer, const Ref<IndexBuffer>& indexBuffer) const = 0;
    virtual Ref<VertexBuffer> createVertexBuffer(const float* vertices, uint32_t size) const = 0;
    virtual Ref<IndexBuffer> createIndexBuffer(const uint32_t* indexes, uint32_t count) const = 0;
    virtual Ref<Shader> createShader(const std::string& vertexPath, const std::string& fragmentPath) const = 0;
    virtual Ref<Texture2D> createTexture2D(const std::string& path) const = 0;
    virtual Ref<Texture2D> createTexture2D(unsigned int channels, unsigned int width, unsigned int height, unsigned int bytesPerPixel, const void* data) const = 0;
    virtual Ref<Texture2D> createTexture2D(unsigned int channels, unsigned int width, unsigned int height, unsigned int bytesPerPixel, TextureFormat format,
                                           const void* data) const = 0;
    virtual Ref<Texture2D> createBRDFLUT(const Ref<Shader>& shader, uint32_t width) const = 0;
    virtual Ref<TextureCube> createTextureCube(const std::array<std::string, 6>& paths) const = 0;
    virtual Ref<TextureCube> createTextureCubeFromHDR(const Ref<Texture2D>& hdrTexture, const Ref<Shader>& convertShader, uint32_t size) = 0;
    virtual Ref<TextureCube> createPrefilteredCubemap(const Ref<TextureCube>& textureCube, const Ref<Shader>& convertShader, uint32_t size) = 0;

    virtual void beginFrame() = 0;
    virtual void beginDirectionalShadows() const = 0;
    virtual void beginPointLightShadows() const = 0;
    virtual void beginPointLightShadow(const PointLight& light, int lightIndex, int faceIndex) const = 0;
    virtual void endShadows() const = 0;
    void beginSelectedMesh() const;
    void endSelectedMesh() const;
    void endMeshes() const;
    virtual void endFrame() const = 0;
    virtual void clear() const = 0;
    virtual void drawToMainFramebuffer() const = 0;

    virtual void enableStencilWriting() const = 0;
    virtual void disableStencilWriting() const = 0;

    void setIrradianceSH(const std::array<glm::vec3, 9>& irradianceSh);
    void setPrefilteredEnvMap(const Ref<TextureCube>& prefilteredEnvMap);
    void setBRDFLUT(const Ref<Texture2D>& brdfLUT);
    // needs to be called before each frame
    void setDirectionalLight(const Ref<DirectionalLight>& directionalLight);
    void addPointLight(const PointLight& pointLight);
    void setDirectionalShadowMapShader(const Ref<Shader>& shadowMapShader);
    void setPointLightShadowMapShader(const Ref<Shader>& shader);

    virtual void draw(unsigned int entityId, const Ref<VertexArray>& vertexArray, const glm::mat4& transform, const Ref<Shader>& shader, int stencil) const = 0;
    virtual void draw(unsigned int entityId, const Ref<VertexArray>& vertexArray, const glm::mat4& transform, const Ref<Shader>& shader, int stencil,
                      const Material& material) const = 0;
    virtual void drawForDirectionalShadows(const Ref<VertexArray>& vertexArray, const glm::mat4& transform) const = 0;
    virtual void drawForPointLightShadows(const Ref<VertexArray>& vertexArray, const glm::mat4& transform) const = 0;
    virtual void drawSelectedMeshOutline(const Ref<VertexArray>& vertexArray, const glm::mat4& transform, const Ref<Shader>& outlineShader) const = 0;
    virtual void drawSkybox(const Ref<SkyboxCube>& skybox) const = 0;
    virtual void drawEditorOverlays(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader) const = 0;
    virtual void drawUI(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const Material& material) const = 0;

protected:
    Ref<Camera> camera;
    glm::mat4 viewProjectionMatrix;
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    Viewport viewport;

    // lighting
    std::array<glm::vec3, 9> irradianceSH = std::array<glm::vec3, 9>();
    Ref<TextureCube> prefilteredEnvMap;
    Ref<Texture2D> brdfLUT;
    Ref<DirectionalLight> directionalLight;
    glm::mat4 directionalLightViewProjection;
    std::vector<PointLight> pointLights = std::vector<PointLight>();

    // shadow mapping
    Ref<DepthFramebuffer> shadowDepthFramebuffer;
    Ref<Shader> shadowMapShader;
    Ref<ShadowCubeArrayFramebuffer> shadowCubeArrayFramebuffer;
    Ref<Shader> shadowCubeArrayShader;

    Ref<DataFramebuffer> dataFramebuffer;

private:
    RenderAPI api;
};
