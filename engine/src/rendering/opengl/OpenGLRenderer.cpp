#include "pch/enginepch.h"
#include "OpenGLRenderer.h"

#include "DebugGroup.h"
#include "OpenGLBuffer.h"
#include "OpenGLPipeline.h"
#include "OpenGLTypes.h"
#include "OpenGLShader.h"
#include "OpenGLTexture.h"
#include "OpenGLTextureCube.h"
#include "OpenGLVertexArray.h"
#include "framebuffer/OpenGLDataFramebuffer.h"
#include "framebuffer/OpenGLDepthFramebuffer.h"
#include "framebuffer/OpenGLRenderFramebuffer.h"
#include "framebuffer/OpenGLRenderPassFramebuffer.h"
#include "framebuffer/OpenGLShadowCubeArrayFramebuffer.h"

#include <glad/gl.h>
#include <glm/glm.hpp>

void OpenGLRenderer::init(const unsigned int width, const unsigned int height) {
    Renderer::init(width, height);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    glFrontFace(GL_CW);
    this->shadowDepthFramebuffer = std::make_shared<OpenGLDepthFramebuffer>(shared_from_this(), 2048, 2048);
    this->shadowCubeArrayFramebuffer = std::make_shared<OpenGLShadowCubeArrayFramebuffer>(shared_from_this(), 1024);
}

void OpenGLRenderer::createRenderFramebuffer(unsigned int width, unsigned int height) {
    this->framebuffer = std::make_shared<OpenGLRenderFramebuffer>(shared_from_this(), width, height);
}

void OpenGLRenderer::createRenderPassFramebuffers(unsigned int width, unsigned int height) {
    this->previousPassFramebuffer = std::make_shared<OpenGLRenderPassFramebuffer>(shared_from_this(), width, height);
    this->currentPassFramebuffer = std::make_shared<OpenGLRenderPassFramebuffer>(shared_from_this(), width, height);
}

void OpenGLRenderer::createDataFramebuffer(unsigned int width, unsigned int height) {
    this->dataFramebuffer = std::make_shared<OpenGLDataFramebuffer>(shared_from_this(), width, height);
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

Ref<Texture> OpenGLRenderer::createBRDFLUT(const Ref<Shader>& shader, const uint32_t size) {
    DebugGroup group("OpenGLRenderer::createBRDFLUT");
    const Ref<OpenGLTexture> texture = std::static_pointer_cast<OpenGLTexture>(
        Texture::builder().size(size).format(TextureFormat::RG).internalFormat(TextureInternalFormat::RG16_FLOAT).build(this->shared_from_this()));
    glBindTexture(GL_TEXTURE_2D, texture->getId());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    uint32_t captureFramebuffer;
    uint32_t captureRenderbuffer;
    glGenFramebuffers(1, &captureFramebuffer);
    glGenRenderbuffers(1, &captureRenderbuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, captureFramebuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, size, size);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->getId(), 0);
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
    return texture;
}

Ref<Texture> OpenGLRenderer::createTextureCubeFromHDR(const Ref<Texture>& hdrTexture, const Ref<Shader>& convertShader, const uint32_t size) {
    return OpenGLTextureCube::createFromHDR(shared_from_this(), hdrTexture, convertShader, size);
}

Ref<Texture> OpenGLRenderer::createPrefilteredCubemap(const Ref<Texture>& textureCube, const Ref<Shader>& convertShader, const uint32_t size) {
    return OpenGLTextureCube::createPrefilteredCubemap(shared_from_this(), textureCube, convertShader, size);
}


void OpenGLRenderer::beginFrame() {
    DE_PROFILE_FUNCTION();
    DebugGroup group("OpenGLRenderer::beginFrame");
    this->viewProjectionMatrix = this->camera->getViewProjectionMatrix(true);
    this->viewMatrix = this->camera->getViewMatrix();
    this->projectionMatrix = this->camera->getProjectionMatrix();
    this->pointLights.clear();
    this->framebuffer->bind();
    this->clear(); // make sure to clean the framebuffer
    // TODO: this should be handled in a render loop based on commands
    auto pipeline = OpenGLPipeline();
    pipeline.params.blending.enabled = true;
    pipeline.params.blending.color.srcFactor = PipelineBlendFactor::SRC_ALPHA;
    pipeline.params.blending.color.dstFactor = PipelineBlendFactor::ONE_MINUS_SRC_ALPHA;
    pipeline.params.blending.alpha.srcFactor = PipelineBlendFactor::SRC_ALPHA;
    pipeline.params.blending.alpha.dstFactor = PipelineBlendFactor::ONE_MINUS_SRC_ALPHA;
    pipeline.bind();
}

void OpenGLRenderer::beginDirectionalShadows() const {
    DebugGroup group("OpenGLRenderer::beginDirectionalShadows");
    this->shadowDepthFramebuffer->bind();
    glViewport(0, 0, this->shadowDepthFramebuffer->getWidth(), this->shadowDepthFramebuffer->getHeight());
    glClear(GL_DEPTH_BUFFER_BIT);
}

void OpenGLRenderer::beginPointLightShadows() const {
    DebugGroup group("OpenGLRenderer::beginPointLightShadows");
    this->shadowCubeArrayFramebuffer->ensureLayersCapacity(static_cast<unsigned int>(this->pointLights.size()));
}

void OpenGLRenderer::beginPointLightShadow(const PointLight& light, const int lightIndex, const int faceIndex) const {
    DebugGroup group("OpenGLRenderer::beginPointLightShadow");
    this->shadowCubeArrayShader->bind();
    this->shadowCubeArrayFramebuffer->bind(lightIndex, faceIndex);
    glViewport(0, 0, this->shadowCubeArrayFramebuffer->getSize(), this->shadowCubeArrayFramebuffer->getSize());
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    const glm::mat4 faceView = TextureCubeUtils::shadowViewMatrices[faceIndex];
    const glm::mat4 projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, light.farPlane);
    const glm::mat4 viewProjection = projection * glm::translate(faceView, -light.position);
    this->shadowCubeArrayShader->uploadUniformMat4("uViewProjection", viewProjection);
    this->shadowCubeArrayShader->uploadUniformVec3("uLightPosition", light.position);
}

void OpenGLRenderer::endShadows() const {
    DebugGroup group("OpenGLRenderer::endShadows");
    this->framebuffer->bind();
    glViewport(this->viewport.x, this->viewport.y, this->viewport.width, this->viewport.height);
}

void OpenGLRenderer::endFrame() const {
    DebugGroup group("OpenGLRenderer::endFrame");
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindVertexArray(0);
}

void OpenGLRenderer::clear() const {
    DebugGroup group("OpenGLRenderer::clear");
    this->framebuffer->clear();
    this->dataFramebuffer->clear();
    this->previousPassFramebuffer->clear();
    this->currentPassFramebuffer->clear();
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glUseProgram(0);
}

void OpenGLRenderer::drawToMainFramebuffer() const {
    this->framebuffer->copyColorToBuffer(0);
}


void OpenGLRenderer::draw(const unsigned int entityId, const Ref<VertexArray>& vertexArray, const glm::mat4& transform, const Ref<Shader>& shader) const {
    this->draw(entityId, vertexArray, transform, shader, Material(this->whitePixelTexture));
}

void OpenGLRenderer::draw(const unsigned int entityId, const Ref<VertexArray>& vertexArray, const glm::mat4& transform, const Ref<Shader>& shader, const Material& material) const {
    DebugGroup group("OpenGLRenderer::draw");
    shader->bind();
    shader->uploadUniformInt("uEntityId", static_cast<int>(entityId));
    shader->uploadUniformMat4("uViewProjection", this->viewProjectionMatrix);
    shader->uploadUniformMat4("uTransform", transform);
    shader->uploadUniformMat4("uDirectionalLightViewProjection", this->directionalLightViewProjection);
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
    shader->uploadUniformInt("uBRDFLUT", textureSlot++);
    this->shadowDepthFramebuffer->getDepthTexture()->bind(textureSlot);
    shader->uploadUniformInt("uDirectionalShadowMap", textureSlot++);
    this->shadowCubeArrayFramebuffer->getShadowCubeArrayTexture()->bind(textureSlot);
    shader->uploadUniformInt("uPointShadowMaps", textureSlot);
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
        shader->uploadUniformFloat("uPointLights[" + std::to_string(i) + "].farPlane", pointLight.farPlane);
    }
    shader->uploadUniformInt("uPointLightsCount", static_cast<int>(this->pointLights.size()));
    // camera position
    shader->uploadUniformVec3("uCameraPosition", this->camera->getPosition());

    vertexArray->bind();
    glDrawElements(GL_TRIANGLES, static_cast<int>(vertexArray->getIndexBuffer()->getCount()), GL_UNSIGNED_INT, nullptr);
}

void OpenGLRenderer::drawSkybox(const Ref<SkyboxCube>& skybox) const {
    DebugGroup group("OpenGLRenderer::drawSkybox");
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
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LESS);
}

void OpenGLRenderer::drawForDirectionalShadows(const Ref<VertexArray>& vertexArray, const glm::mat4& transform) const {
    DebugGroup group("OpenGLRenderer::drawForDirectionalShadows");
    this->shadowMapShader->bind();
    this->shadowMapShader->uploadUniformMat4("uViewProjection", this->directionalLightViewProjection);
    this->shadowMapShader->uploadUniformMat4("uTransform", transform);
    vertexArray->bind();
    glDrawElements(GL_TRIANGLES, static_cast<int>(vertexArray->getIndexBuffer()->getCount()), GL_UNSIGNED_INT, nullptr);
}

void OpenGLRenderer::drawForPointLightShadows(const Ref<VertexArray>& vertexArray, const glm::mat4& transform) const {
    DebugGroup group("OpenGLRenderer::drawForPointLightShadows");
    // the shader is already initialized in beginPointLightShadow(s)
    this->shadowCubeArrayShader->uploadUniformMat4("uTransform", transform);
    vertexArray->bind();
    glDrawElements(GL_TRIANGLES, static_cast<int>(vertexArray->getIndexBuffer()->getCount()), GL_UNSIGNED_INT, nullptr);
}

void OpenGLRenderer::drawJumpFloodingPrepare(const Ref<VertexArray>& vertexArray, const glm::mat4& transform, const Ref<Shader>& outlineShader) const {
    DebugGroup group("OpenGLRenderer::drawJumpFloodingPrepare");
    this->currentPassFramebuffer->bind();
    outlineShader->bind();
    outlineShader->uploadUniformMat4("uViewProjection", this->viewProjectionMatrix);
    outlineShader->uploadUniformMat4("uTransform", transform);
    vertexArray->bind();
    glDrawElements(GL_TRIANGLES, static_cast<int>(vertexArray->getIndexBuffer()->getCount()), GL_UNSIGNED_INT, nullptr);
    this->framebuffer->bind();
}

void OpenGLRenderer::drawJumpFloodingPass(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const int offset, const bool vertical) {
    DebugGroup group("OpenGLRenderer::drawJumpFloodingPass");
    this->swapPassFramebuffers();
    this->currentPassFramebuffer->bind();
    shader->bind();
    this->previousPassFramebuffer->getDepthTexture()->bind(0);
    shader->uploadUniformInt("uPassDepthTexture", 0);
    this->previousPassFramebuffer->getTexture()->bind(1);
    shader->uploadUniformInt("uPassTexture", 1);
    shader->uploadUniformInt("uOffset", offset);
    shader->uploadUniformVec2Int("uDirection", vertical ? glm::ivec2(0, 1) : glm::ivec2(1, 0));
    vertexArray->bind();
    glDepthFunc(GL_ALWAYS);
    glViewport(0, 0, this->framebuffer->getWidth(), this->framebuffer->getHeight());
    glDrawElements(GL_TRIANGLES, static_cast<int>(vertexArray->getIndexBuffer()->getCount()), GL_UNSIGNED_INT, nullptr);
    glDepthFunc(GL_LESS);
}

void OpenGLRenderer::drawEditorOverlays(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const glm::vec4 outlineColor, const float outlineWidth) const {
    DebugGroup group("OpenGLRenderer::drawEditorOverlays");
    this->framebuffer->bind();
    shader->bind();
    this->framebuffer->getDepthTexture()->bind(0);
    shader->uploadUniformInt("uMainDepthTexture", 0);
    this->currentPassFramebuffer->getDepthTexture()->bind(1);
    shader->uploadUniformInt("uPassDepthTexture", 1);
    this->currentPassFramebuffer->getTexture()->bind(2);
    shader->uploadUniformInt("uPassTexture", 2);
    shader->uploadUniformVec4("uOutlineColor", outlineColor);
    shader->uploadUniformFloat("uOutlineWidth", outlineWidth);
    vertexArray->bind();
    glDisable(GL_DEPTH_TEST);
    glViewport(0, 0, this->framebuffer->getWidth(), this->framebuffer->getHeight());
    glDrawElements(GL_TRIANGLES, static_cast<int>(vertexArray->getIndexBuffer()->getCount()), GL_UNSIGNED_INT, nullptr);
    glEnable(GL_DEPTH_TEST);
}

void OpenGLRenderer::drawUI(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const Material& material) const {
    DebugGroup group("OpenGLRenderer::drawUI");
    shader->bind();
    material.albedo->bind(0);
    shader->uploadUniformInt("uTexture", 0);
    vertexArray->bind();
    glDisable(GL_DEPTH_TEST);
    glViewport(0, 0, this->framebuffer->getWidth(), this->framebuffer->getHeight());
    glDrawElements(GL_TRIANGLES, static_cast<int>(vertexArray->getIndexBuffer()->getCount()), GL_UNSIGNED_INT, nullptr);
    glEnable(GL_DEPTH_TEST);
}

Ref<Texture> OpenGLRenderer::newTexture(const Texture::TextureParams& params) {
    return std::make_shared<OpenGLTexture>(params, this->shared_from_this());
}
