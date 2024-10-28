#include "pch/enginepch.h"
#include "OpenGLRenderer.h"

#include "OpenGLBuffer.h"
#include "OpenGLShader.h"
#include "OpenGLTexture2D.h"
#include "OpenGLTextureCube.h"
#include "OpenGLVertexArray.h"
#include "framebuffer/OpenGLDepthFramebuffer.h"
#include "framebuffer/OpenGLRenderFramebuffer.h"

#include <glad/gl.h>
#include <glm/glm.hpp>

void OpenGLRenderer::init(const int x, const int y, const uint32_t width, const uint32_t height) {
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    glFrontFace(GL_CW);
    this->setViewport(x, y, width, height);
    unsigned char white[4] = {255, 255, 255, 255};
    this->whitePixelTexture = std::make_shared<OpenGLTexture2D>(1, 1, 1, 1, white);
    this->defaultOcclusionRoughnessMetallicTexture = std::make_shared<OpenGLTexture2D>(3, 1, 1, 1, std::array<unsigned char, 3>{255, 255, 0}.data());
    this->shadowDepthFramebuffer = std::make_shared<OpenGLDepthFramebuffer>(2048, 2048);
}

void OpenGLRenderer::setViewport(const int x, const int y, const uint32_t width, const uint32_t height) {
    glViewport(x, y, static_cast<int>(width), static_cast<int>(height));
    if (this->camera) {
        this->camera->setAspectRatio(static_cast<float>(width) / static_cast<float>(height));
    }
    this->framebuffer = std::make_shared<OpenGLRenderFramebuffer>(width, height);
}

const Ref<RenderFramebuffer>& OpenGLRenderer::getFramebuffer() const {
    return this->framebuffer;
}


Ref<VertexArray> OpenGLRenderer::createVertexArray(const Ref<VertexBuffer>& vertexBuffer, const Ref<IndexBuffer>& indexBuffer) const {
    return std::make_shared<OpenGLVertexArray>(vertexBuffer, indexBuffer);
}

Ref<VertexBuffer> OpenGLRenderer::createVertexBuffer(const float* vertices, const uint32_t size) const {
    return std::make_shared<OpenGLVertexBuffer>(vertices, size);
}

Ref<IndexBuffer> OpenGLRenderer::createIndexBuffer(const uint32_t* indexes, const uint32_t count) const {
    return std::make_shared<OpenGLIndexBuffer>(indexes, count);
}

Ref<Shader> OpenGLRenderer::createShader(const std::string& vertexPath, const std::string& fragmentPath) const {
    return std::make_shared<OpenGLShader>(vertexPath, fragmentPath);
}

Ref<Texture2D> OpenGLRenderer::createTexture2D(const std::string& path) const {
    return std::make_shared<OpenGLTexture2D>(path);
}

Ref<Texture2D> OpenGLRenderer::createTexture2D(const unsigned int channels, const unsigned int width, const unsigned int height, const unsigned int bytesPerPixel,
                                               const void* data) const {
    return std::make_shared<OpenGLTexture2D>(channels, width, height, bytesPerPixel, data);
}

Ref<Texture2D> OpenGLRenderer::createBRDFLUT(const Ref<Shader>& shader, const uint32_t size) const {
    unsigned int textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, size, size, 0, GL_RG, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    uint32_t captureFramebuffer;
    uint32_t captureRenderbuffer;
    glGenFramebuffers(1, &captureFramebuffer);
    glGenRenderbuffers(1, &captureRenderbuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, captureFramebuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, size, size);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRenderbuffer);

    int previousViewport[4];
    glGetIntegerv(GL_VIEWPORT, previousViewport);

    glViewport(0, 0, size, size);
    shader->bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    constexpr float vertices[] = {
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, //
        1.0f,  -1.0f, 0.0f, 1.0f, 0.0f, //
        1.0f,  1.0f,  0.0f, 1.0f, 1.0f, //
        -1.0f, 1.0f,  0.0f, 0.0f, 1.0f, //
    };
    const Ref<VertexBuffer> vertexBuffer = this->createVertexBuffer(vertices, sizeof(vertices));
    vertexBuffer->setLayout({
        {DataType::FLOAT3, "position"},
        {DataType::FLOAT2, "textureCoords"},
    });
    const uint32_t indices[] = {0, 1, 2, 2, 3, 0};
    const Ref<IndexBuffer> indexBuffer = this->createIndexBuffer(indices, 6);
    const Ref<VertexArray> vertexArray = this->createVertexArray(vertexBuffer, indexBuffer);
    vertexArray->bind();
    glDisable(GL_BLEND);
    glDrawElements(GL_TRIANGLES, static_cast<int>(vertexArray->getIndexBuffer()->getCount()), GL_UNSIGNED_INT, nullptr);
    glEnable(GL_BLEND);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(previousViewport[0], previousViewport[1], previousViewport[2], previousViewport[3]);
    glDeleteFramebuffers(1, &captureFramebuffer);
    glDeleteRenderbuffers(1, &captureRenderbuffer);
    return std::make_shared<OpenGLTexture2D>(textureId, size, size);
}

Ref<TextureCube> OpenGLRenderer::createTextureCube(const std::array<std::string, 6>& paths) const {
    return std::make_shared<OpenGLTextureCube>(paths);
}

Ref<TextureCube> OpenGLRenderer::createTextureCubeFromHDR(const Ref<Texture2D>& hdrTexture, const Ref<Shader>& convertShader, const uint32_t size) {
    return OpenGLTextureCube::createFromHDR(shared_from_this(), hdrTexture, convertShader, size);
}

Ref<TextureCube> OpenGLRenderer::createPrefilteredCubemap(const Ref<TextureCube>& textureCube, const Ref<Shader>& convertShader, const uint32_t size) {
    return OpenGLTextureCube::createPrefilteredCubemap(shared_from_this(), textureCube, convertShader, size);
}


void OpenGLRenderer::beginFrame() {
    DE_PROFILE_FUNCTION();
    this->viewProjectionMatrix = this->camera->getViewProjectionMatrix(true);
    this->viewMatrix = this->camera->getViewMatrix();
    this->projectionMatrix = this->camera->getProjectionMatrix();
    this->pointLights.clear();
    this->framebuffer->bind();
    this->clean(); // make sure to clean the framebuffer
    glEnable(GL_CULL_FACE); // disabled by the skybox
    glDepthFunc(GL_LESS); // changed by the skybox
}

void OpenGLRenderer::endFrame() const {
    DE_PROFILE_FUNCTION();
    this->framebuffer->updateFinalColorTexture();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLRenderer::clean() const {
    DE_PROFILE_FUNCTION();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRenderer::draw(const Ref<VertexArray>& vertexArray, const glm::mat4& transform, const Ref<Shader>& shader) const {
    this->draw(vertexArray, transform, shader, Material(this->whitePixelTexture));
}

void OpenGLRenderer::draw(const Ref<VertexArray>& vertexArray, const glm::mat4& transform, const Ref<Shader>& shader, const Material& material) const {
    shader->bind();
    shader->uploadUniformMat4("uViewProjection", this->viewProjectionMatrix);
    shader->uploadUniformMat4("uTransform", transform);
    // texture
    int textureSlot = 0;
    material.albedo->bind(textureSlot);
    shader->uploadUniformInt("uAlbedo", textureSlot++);
    if (material.occlusionRoughnessMetallic) {
        material.occlusionRoughnessMetallic->bind(textureSlot);
    } else {
        this->defaultOcclusionRoughnessMetallicTexture->bind(textureSlot);
    }
    shader->uploadUniformInt("uOcclusionRoughnessMetallic", textureSlot++);
    this->prefilteredEnvMap->bind(textureSlot);
    shader->uploadUniformInt("uPrefilteredEnvMap", textureSlot++);
    this->brdfLUT->bind(textureSlot);
    shader->uploadUniformInt("uBRDFLUT", textureSlot);
    // irradiance spherical harmonics
    for (int i = 0; i < this->irradianceSH.size(); i++) {
        shader->uploadUniformVec3("uIrradianceSH[" + std::to_string(i) + "]", this->irradianceSH[i]);
    }
    // lights
    shader->uploadUniformInt("uMaterial.ignoreLighting", material.ignoreLighting);
    shader->uploadUniformVec3("uDirectionalLight.color", {1.0f, 1.0f, 1.0f});
    shader->uploadUniformFloat("uDirectionalLight.intensity", this->directionalLight->getIntensity());
    shader->uploadUniformVec3("uDirectionalLight.direction", this->directionalLight->getRotation().toDirection());
    for (size_t i = 0; i < this->pointLights.size(); i++) {
        const PointLight& pointLight = this->pointLights[i];
        shader->uploadUniformVec3("uPointLights[" + std::to_string(i) + "].position", pointLight.position);
        shader->uploadUniformVec3("uPointLights[" + std::to_string(i) + "].color", pointLight.color);
        shader->uploadUniformFloat("uPointLights[" + std::to_string(i) + "].intensity", pointLight.intensity);
    }
    shader->uploadUniformInt("uPointLightsCount", static_cast<int>(this->pointLights.size()));
    // camera position
    shader->uploadUniformVec3("uCameraPosition", this->camera->getPosition());

    vertexArray->bind();
    glDrawElements(GL_TRIANGLES, static_cast<int>(vertexArray->getIndexBuffer()->getCount()), GL_UNSIGNED_INT, nullptr);
}

void OpenGLRenderer::drawSkybox(const Ref<SkyboxCube>& skybox) const {
    skybox->getShader()->bind();
    // remove the translation part of the view matrix
    const auto viewMatrix = glm::mat4(glm::mat3(this->viewMatrix));
    const auto viewProjectionMatrix = this->projectionMatrix * viewMatrix;
    skybox->getShader()->uploadUniformMat4("uViewProjection", viewProjectionMatrix);
    skybox->getTexture()->bind(0);
    skybox->getShader()->uploadUniformInt("uSkybox", 0);
    skybox->getVertexArray()->bind();
    glDepthFunc(GL_LEQUAL);
    glDisable(GL_CULL_FACE);
    glDrawElements(GL_TRIANGLES, static_cast<int>(skybox->getVertexArray()->getIndexBuffer()->getCount()), GL_UNSIGNED_INT, nullptr);
}
