#include "pch/enginepch.h"
#include "OpenGLRenderer.h"

#include "OpenGLBuffer.h"
#include "OpenGLFramebuffer.h"
#include "OpenGLShader.h"
#include "OpenGLTexture2D.h"
#include "OpenGLTextureCube.h"
#include "OpenGLVertexArray.h"

#include <glad/gl.h>
#include <glm/glm.hpp>

void OpenGLRenderer::init(const int x, const int y, const uint32_t width, const uint32_t height) {
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glFrontFace(GL_CW);
    this->setViewport(x, y, width, height);
    unsigned char white[4] = {255, 255, 255, 255};
    this->whitePixelTexture = std::make_shared<OpenGLTexture2D>(1, 1, 1, 1, white);
    this->defaultOcclusionRoughnessMetallicTexture = std::make_shared<OpenGLTexture2D>(3, 1, 1, 1, std::array<unsigned char, 3>{255, 255, 0}.data());
}

void OpenGLRenderer::setViewport(const int x, const int y, const uint32_t width, const uint32_t height) {
    glViewport(x, y, static_cast<int>(width), static_cast<int>(height));
    if (this->camera) {
        this->camera->setAspectRatio(static_cast<float>(width) / static_cast<float>(height));
    }
    this->framebuffer = std::make_shared<OpenGLFramebuffer>(width, height);
}

const Ref<Framebuffer>& OpenGLRenderer::getFramebuffer() const {
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

Ref<Texture2D> OpenGLRenderer::createTexture2D(const unsigned int channels, const unsigned int width, const unsigned int height, const unsigned int bytesPerPixel, const void* data) const {
    return std::make_shared<OpenGLTexture2D>(channels, width, height, bytesPerPixel, data);
}

Ref<TextureCube> OpenGLRenderer::createTextureCube(const std::array<std::string, 6>& paths) const {
    return std::make_shared<OpenGLTextureCube>(paths);
}

Ref<TextureCube> OpenGLRenderer::createTextureCubeFromHDR(const Ref<Texture2D>& hdrTexture, const Ref<Shader>& convertShader, const uint32_t size) {
    return OpenGLTextureCube::createFromHDR(shared_from_this(), hdrTexture, convertShader, size);
}

Ref<TextureCube> OpenGLRenderer::createIrradianceMap(const Ref<TextureCube>& textureCube, const Ref<Shader>& irradianceShader, const uint32_t size) {
    return OpenGLTextureCube::createIrradianceMap(shared_from_this(), textureCube, irradianceShader, size);
}


void OpenGLRenderer::beginFrame() {
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
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLRenderer::clean() const {
    DE_PROFILE_FUNCTION();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRenderer::addPointLight(const PointLight& pointLight) {
    this->pointLights.push_back(pointLight);
}

void OpenGLRenderer::draw(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const glm::mat4& transform, const Ref<DirectionalLight>& directionalLight) const {
    this->draw(vertexArray, shader, transform, directionalLight, Material(this->whitePixelTexture));
}

void OpenGLRenderer::draw(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const glm::mat4& transform, const Ref<DirectionalLight>& directionalLight,
                          const Material& material) const {
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
    shader->uploadUniformInt("uOcclusionRoughnessMetallic", textureSlot);
    // irradiance spherical harmonics
    for (int i = 0; i < this->irradianceSH.size(); i++) {
        shader->uploadUniformVec3("uIrradianceSH[" + std::to_string(i) + "]", this->irradianceSH[i]);
    }
    // lights
    shader->uploadUniformInt("uMaterial.ignoreLighting", material.ignoreLighting);
    shader->uploadUniformVec3("uDirectionalLight.color", {1.0f, 1.0f, 1.0f});
    shader->uploadUniformFloat("uDirectionalLight.intensity", directionalLight->getIntensity());
    shader->uploadUniformVec3("uDirectionalLight.direction", directionalLight->getLocalDirection(transform));
    for (size_t i = 0; i < this->pointLights.size(); i++) {
        const PointLight& pointLight = this->pointLights[i];
        shader->uploadUniformVec3("uPointLights[" + std::to_string(i) + "].position", glm::vec3(inverse(transform) * glm::vec4(pointLight.position, 1.0f)));
        shader->uploadUniformVec3("uPointLights[" + std::to_string(i) + "].color", pointLight.color);
        shader->uploadUniformFloat("uPointLights[" + std::to_string(i) + "].intensity", pointLight.intensity);
    }
    shader->uploadUniformInt("uPointLightsCount", static_cast<int>(this->pointLights.size()));
    // camera position
    const auto cameraPositionLocal = glm::vec3(inverse(transform) * glm::vec4(this->camera->getPosition(), 1.0f));
    shader->uploadUniformVec3("uCameraPositionLocal", cameraPositionLocal);

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
