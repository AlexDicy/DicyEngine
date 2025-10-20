#include "pch/enginepch.h"
#include "Texture.h"

TextureBuilder Texture::builder() {
    return {};
}

Texture::Texture(const TextureParams& params, const Ref<Renderer>& renderer) : params(params), renderer(renderer) {
    if (this->params.type == TextureType::TEXTURE_CUBE) {
        DE_ASSERT(this->params.layers == 6, "Cubemap must have 6 layers")
        DE_ASSERT(this->params.width == this->params.height, "Cubemap must be square")
    } else if (this->params.type == TextureType::TEXTURE_CUBE_ARRAY) {
        DE_ASSERT(this->params.layers % 6 == 0, "Cubemap array must have a multiple of 6 layers")
        DE_ASSERT(this->params.width == this->params.height, "Cubemap array must be square")
    }
}

Texture::~Texture() {
    getRenderer()->destroyTexture(*this);
}

void Texture::bind(const unsigned int slot) const {
    getRenderer()->bindTexture(shared_from_this(), slot);
}

void Texture::resize(const unsigned int width, const unsigned int height) {
    params.width = width;
    params.height = height;
    getRenderer()->createTextureStorage(shared_from_this(), nullptr);
}

void Texture::resize(const unsigned int width, const unsigned int height, const unsigned int layers) {
    DE_ASSERT(params.type == TextureType::TEXTURE_2D_ARRAY || params.type == TextureType::TEXTURE_CUBE_ARRAY,
              "Resize with layers is only supported for 2D Array and Cube Array textures")
    DE_ASSERT(layers % 6 == 0 || params.type == TextureType::TEXTURE_2D_ARRAY, "Cube Array textures must have a multiple of 6 layers")
    params.width = width;
    params.height = height;
    params.layers = layers;
    getRenderer()->createTextureStorage(shared_from_this(), nullptr);
}

Ref<CubeMap> Texture::toCubemap() const {
    DE_ASSERT(params.type == TextureType::TEXTURE_CUBE, "toCubemap is only supported for Cube textures")
    return getRenderer()->copyTextureToCubeMap(shared_from_this());
}

TextureBuilder& TextureBuilder::fromImage(const Ref<Image>& image) {
    this->width(image->getWidth());
    this->height(image->getHeight());
    this->format(image->getFormat());
    this->internalFormat(image->getInternalFormat());
    auto data = std::make_unique<uint8_t[]>(image->getDataSize());
    std::memcpy(data.get(), image->getData().get(), image->getDataSize());
    this->data(std::move(data));
    return *this;
}

Ref<Texture> TextureBuilder::build(const Ref<Renderer>& renderer) {
    return renderer->createTexture(this->params, std::move(this->textureData));
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
