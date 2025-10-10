#include "pch/enginepch.h"
#include "Texture.h"

TextureBuilder Texture::builder() {
    return {};
}

Texture::Texture(const unsigned int width, const unsigned int height, const unsigned int layers, const TextureFormat format, const TextureInternalFormat internalFormat, const TextureType type) :
    width(width), height(height), layers(layers), format(format), internalFormat(internalFormat), type(type) {
    if (this->type == TextureType::TEXTURE_CUBE) {
        DE_ASSERT(this->layers == 6, "Cubemap must have 6 layers")
        DE_ASSERT(this->width == this->height, "Cubemap must be square")
    } else if (this->type == TextureType::TEXTURE_CUBE_ARRAY) {
        DE_ASSERT(this->layers % 6 == 0, "Cubemap array must have a multiple of 6 layers")
        DE_ASSERT(this->width == this->height, "Cubemap array must be square")
    }
}

TextureBuilder& TextureBuilder::fromImage(const Ref<Image>& image) {
    this->width(image->getWidth());
    this->height(image->getHeight());
    this->format(image->getFormat());
    this->internalFormat(image->getInternalFormat());
    this->data(image->getData());
    return *this;
}

Ref<Texture> TextureBuilder::build(const Ref<const Renderer>& renderer) const {
    const auto [width, height, layers, format, internalFormat, type, data] = this->builderData;
    return renderer->createTexture(width, height, layers, format, internalFormat, type, data);
}

const glm::mat4 TextureCubeUtils::invertedViewMatrices[] = {
    lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)), // right
    lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)), // left
    lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)), // top
    lookAt(glm::vec3(0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f)), // bottom
    lookAt(glm::vec3(0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)), // front
    lookAt(glm::vec3(0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)), // back
};

const glm::mat4 TextureCubeUtils::shadowViewMatrices[] = {
    lookAt(glm::vec3(0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)), // right
    lookAt(glm::vec3(0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)), // left
    lookAt(glm::vec3(0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)), // bottom
    lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)), // top
    lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f)), // front
    lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f)), // back
};
