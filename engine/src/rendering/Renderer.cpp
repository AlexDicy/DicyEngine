#include "pch/enginepch.h"
#include "Renderer.h"

#include "RenderCommands.h"
#include "images/CubeMap.h"
#include "images/ImageUtils.h"

void Renderer::init(const unsigned int width, const unsigned int height) {
    this->setFramebufferDimensions(width, height);
    this->setViewport(0, 0, width, height);
    auto white = std::make_unique<uint8_t[]>(4);
    std::memcpy(white.get(), std::array<uint8_t, 4>{255, 255, 255, 255}.data(), 4);
    this->whitePixelTexture =
        Texture::builder().size(1).format(TextureFormat::RGBA).internalFormat(TextureInternalFormat::RGBA8).data(std::move(white)).build(this->shared_from_this());
    auto occlusionRoughnessMetallic = std::make_unique<uint8_t[]>(3);
    std::memcpy(occlusionRoughnessMetallic.get(), std::array<uint8_t, 3>{255, 255, 0}.data(), 3);
    this->defaultOcclusionRoughnessMetallicTexture = Texture::builder()
                                                         .size(1)
                                                         .format(TextureFormat::RGB)
                                                         .internalFormat(TextureInternalFormat::RGB8)
                                                         .data(std::move(occlusionRoughnessMetallic))
                                                         .build(this->shared_from_this());
}

void Renderer::setFramebufferDimensions(const unsigned int width, const unsigned int height) {
    this->createRenderFramebuffer(width, height);
    this->createDataFramebuffer(width, height);
    this->createRenderPassFramebuffers(width, height);
}

void Renderer::setViewport(const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height) {
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

Ref<Texture> Renderer::createTexture(const Texture::TextureParams& params, std::unique_ptr<uint8_t[]> data) {
    Ref<Texture> texture = newTexture(params);
    initializeTexture(texture);
    createTextureStorage(texture, std::move(data));
    return texture;
}

void Renderer::initializeTexture(const Ref<Texture>& texture) {
    pushCommand([texture](const RenderCommands* commands) {
        commands->initializeTexture(texture);
    });
}

void Renderer::createTextureStorage(const Ref<Texture>& texture, std::unique_ptr<uint8_t[]> data) {
    std::shared_ptr dataRef = std::move(data);
    pushCommand([texture, dataRef](const RenderCommands* commands) {
        commands->createTextureStorage(texture, dataRef);
    });
}

void Renderer::bindTexture(const Ref<const Texture>& texture, const unsigned int slot) {
    pushCommand([texture, slot](const RenderCommands* commands) {
        commands->bindTexture(texture, slot);
    });
}

Ref<CubeMap> Renderer::copyTextureToCubeMap(const Ref<const Texture>& texture) {
    std::array<Image, 6> faces;
    pushCommandSync([texture](const RenderCommands* commands) {
        commands->bindTexture(texture);
    });
    for (int i = 0; i < 6; i++) {
        faces[i] = Image(texture->getWidth(), texture->getHeight(), texture->getFormat(), texture->getInternalFormat());
        pushCommandSync([texture, i, &faces](const RenderCommands* commands) {
            commands->copyTextureData(texture, i, faces[i].getData().get());
        });
    }
    return std::make_shared<CubeMap>(std::move(faces));
}

Ref<Texture> Renderer::createTextureCube(const std::array<std::string, 6>& paths) {
    const Ref<Image> firstFace = ImageUtils::loadImageFromFile(paths[0]);
    const size_t dataSize = firstFace->getDataSize();
    auto data = std::make_unique<uint8_t[]>(dataSize * 6);
    std::memcpy(data.get(), firstFace->getData().get(), dataSize);

    for (size_t i = 1; i < paths.size(); i++) {
        const Ref<Image> face = ImageUtils::loadImageFromFile(paths[i]);
        if (face->getWidth() != firstFace->getWidth() || face->getHeight() != firstFace->getHeight() || face->getFormat() != firstFace->getFormat() ||
            face->getInternalFormat() != firstFace->getInternalFormat()) {
            DE_ERROR("All images must have the same dimensions and format to create a cubemap texture");
            return nullptr;
        }
        std::memcpy(data.get() + i * dataSize, face->getData().get(), face->getDataSize());
    }

    Ref<Texture> textureCube = Texture::builder()
                                   .size(firstFace->getWidth())
                                   .layers(6)
                                   .format(firstFace->getFormat())
                                   .internalFormat(firstFace->getInternalFormat())
                                   .type(TextureType::TEXTURE_CUBE)
                                   .data(std::move(data))
                                   .build(this->shared_from_this());
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
