#pragma once
#include "camera/Camera.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include "framebuffer/DepthFramebuffer.h"
#include "framebuffer/RenderFramebuffer.h"
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

    const Ref<Camera>& getCamera() {
        return this->camera;
    }

    void setCamera(const Ref<Camera>& camera) {
        this->camera = camera;
    }

    virtual void init(int x, int y, uint32_t width, uint32_t height) = 0;
    virtual void setViewport(int x, int y, uint32_t width, uint32_t height) = 0;
    virtual const Ref<RenderFramebuffer>& getFramebuffer() const = 0;

    virtual Ref<VertexArray> createVertexArray(const Ref<VertexBuffer>& vertexBuffer, const Ref<IndexBuffer>& indexBuffer) const = 0;
    virtual Ref<VertexBuffer> createVertexBuffer(const float* vertices, uint32_t size) const = 0;
    virtual Ref<IndexBuffer> createIndexBuffer(const uint32_t* indexes, uint32_t count) const = 0;
    virtual Ref<Shader> createShader(const std::string& vertexPath, const std::string& fragmentPath) const = 0;
    virtual Ref<Texture2D> createTexture2D(const std::string& path) const = 0;
    virtual Ref<Texture2D> createTexture2D(unsigned int channels, unsigned int width, unsigned int height, unsigned int bytesPerPixel, const void* data) const = 0;
    virtual Ref<Texture2D> createBRDFLUT(const Ref<Shader>& shader, uint32_t width) const = 0;
    virtual Ref<TextureCube> createTextureCube(const std::array<std::string, 6>& paths) const = 0;
    virtual Ref<TextureCube> createTextureCubeFromHDR(const Ref<Texture2D>& hdrTexture, const Ref<Shader>& convertShader, uint32_t size) = 0;
    virtual Ref<TextureCube> createPrefilteredCubemap(const Ref<TextureCube>& textureCube, const Ref<Shader>& convertShader, uint32_t size) = 0;

    virtual void beginFrame() = 0;
    virtual void endFrame() const = 0;
    virtual void clean() const = 0;
    void setIrradianceSH(const std::array<glm::vec3, 9>& irradianceSh);
    void setPrefilteredEnvMap(const Ref<TextureCube>& prefilteredEnvMap);
    void setBRDFLUT(const Ref<Texture2D>& brdfLUT);
    virtual void addPointLight(const PointLight& pointLight) = 0;
    virtual void draw(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const glm::mat4& transform, const Ref<DirectionalLight>& directionalLight) const = 0;
    virtual void draw(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const glm::mat4& transform, const Ref<DirectionalLight>& directionalLight,
                      const Material& material) const = 0;
    virtual void drawSkybox(const Ref<SkyboxCube>& skybox) const = 0;

protected:
    Ref<Camera> camera;
    glm::mat4 viewProjectionMatrix;
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

    // lighting
    std::array<glm::vec3, 9> irradianceSH = std::array<glm::vec3, 9>();
    Ref<TextureCube> prefilteredEnvMap;
    Ref<Texture2D> brdfLUT;
    std::vector<PointLight> pointLights = std::vector<PointLight>();

    // shadow mapping
    Ref<DepthFramebuffer> shadowDepthFramebuffer;

private:
    RenderAPI api;
};
