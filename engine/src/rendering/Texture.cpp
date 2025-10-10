#include "pch/enginepch.h"
#include "Texture.h"

TextureBuilder Texture::builder() {
    return {};
}

Texture::Texture(const TextureParams& params) : params(params) {
    if (this->params.type == TextureType::TEXTURE_CUBE) {
        DE_ASSERT(this->params.layers == 6, "Cubemap must have 6 layers")
        DE_ASSERT(this->params.width == this->params.height, "Cubemap must be square")
    } else if (this->params.type == TextureType::TEXTURE_CUBE_ARRAY) {
        DE_ASSERT(this->params.layers % 6 == 0, "Cubemap array must have a multiple of 6 layers")
        DE_ASSERT(this->params.width == this->params.height, "Cubemap array must be square")
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
    return renderer->createTexture(this->params, this->textureData);
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
