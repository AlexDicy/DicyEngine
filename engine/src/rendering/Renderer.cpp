#include "pch/enginepch.h"
#include "Renderer.h"

#include "images/ImageUtils.h"


void Renderer::setViewport(const int x, const int y, const uint32_t width, const uint32_t height) {
    if (this->camera) {
        this->camera->setAspectRatio(static_cast<float>(width) / static_cast<float>(height));
    }
    this->viewport.x = x;
    this->viewport.y = y;
    this->viewport.width = static_cast<int>(width);
    this->viewport.height = static_cast<int>(height);
}

const Ref<DepthFramebuffer>& Renderer::getShadowDepthFramebuffer() const {
    return this->shadowDepthFramebuffer;
}

void Renderer::swapPassFramebuffers() {
    const Ref<RenderPassFramebuffer> temp = this->previousPassFramebuffer;
    this->previousPassFramebuffer = this->currentPassFramebuffer;
    this->currentPassFramebuffer = temp;
    this->previousPassFramebuffer->copyDepthToBuffer(this->currentPassFramebuffer);
}

Ref<Texture> Renderer::createTextureCube(const std::array<std::string, 6>& paths) const {
    const Ref<Image> firstFace = ImageUtils::loadImageFromFile(paths[0]);
    const size_t dataSize = firstFace->getDataSize();
    const auto data = new uint8_t[dataSize * 6];
    memcpy(data, firstFace->getData(), dataSize);

    for (size_t i = 1; i < paths.size(); i++) {
        const Ref<Image> face = ImageUtils::loadImageFromFile(paths[i]);
        if (face->getWidth() != firstFace->getWidth() || face->getHeight() != firstFace->getHeight() || face->getFormat() != firstFace->getFormat() ||
            face->getInternalFormat() != firstFace->getInternalFormat()) {
            DE_ERROR("All images must have the same dimensions and format to create a cubemap texture");
            delete[] data;
            return nullptr;
            }
        memcpy(data + i * dataSize, face->getData(), face->getDataSize());
    }

    Ref<Texture> textureCube =
        this->createTexture(firstFace->getWidth(), firstFace->getHeight(), 6, firstFace->getFormat(), firstFace->getInternalFormat(), Texture::TextureType::TEXTURE_CUBE, data);
    delete[] data;
    return textureCube;
}

void Renderer::endMeshes() const {
    this->getFramebuffer()->saveMousePicking();
}

void Renderer::setIrradianceSH(const std::array<glm::vec3, 9>& irradianceSh) {
    this->irradianceSH = irradianceSh;
}

void Renderer::setPrefilteredEnvMap(const Ref<Texture>& prefilteredEnvMap) {
    this->prefilteredEnvMap = prefilteredEnvMap;
}

void Renderer::setBRDFLUT(const Ref<Texture>& brdfLUT) {
    this->brdfLUT = brdfLUT;
}

void Renderer::setDirectionalLight(const Ref<DirectionalLight>& directionalLight) {
    this->directionalLight = directionalLight;
    this->directionalLightViewProjection = directionalLight->getViewProjectionMatrix();
}

void Renderer::addPointLight(const PointLight& pointLight) {
    this->pointLights.push_back(pointLight);
}

void Renderer::setDirectionalShadowMapShader(const Ref<Shader>& shadowMapShader) {
    this->shadowMapShader = shadowMapShader;
}

void Renderer::setPointLightShadowMapShader(const Ref<Shader>& shader) {
    this->shadowCubeArrayShader = shader;
}
