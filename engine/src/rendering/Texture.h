#pragma once
#include "TextureFormats.h"


class CubeMap;
class Renderer;
class TextureBuilder;

class Texture {
public:
    using Format = TextureFormats::Format;
    using InternalFormat = TextureFormats::InternalFormat;
    using DataType = TextureFormats::DataType;
    using TextureType = TextureFormats::Type;

    virtual ~Texture() = default;

    unsigned int getWidth() const {
        return this->width;
    }

    unsigned int getHeight() const {
        return this->height;
    }

    unsigned int getLayers() const {
        return this->layers;
    }

    virtual void bind(unsigned int) const = 0;

    virtual void setRawData(const void* data, unsigned int size) = 0;
    virtual void resize(unsigned int width, unsigned int height) = 0;
    virtual void resize(unsigned int width, unsigned int height, unsigned int layers) = 0;

    virtual Ref<CubeMap> toCubemap() const = 0;

    static TextureBuilder builder();

protected:
    Texture(unsigned int width, unsigned int height, unsigned int layers, Format format, InternalFormat internalFormat, TextureType type);

    unsigned int width;
    unsigned int height;
    unsigned int layers;
    Format format;
    InternalFormat internalFormat;
    TextureType type;
};


class TextureCubeUtils {
public:
    static const glm::mat4 invertedViewMatrices[6];
    static const glm::mat4 shadowViewMatrices[6];
};


class TextureBuilder {
public:
    TextureBuilder& width(const unsigned int width) {
        this->builderData.width = width;
        return *this;
    }

    TextureBuilder& height(const unsigned int height) {
        this->builderData.height = height;
        return *this;
    }

    TextureBuilder& layers(const unsigned int layers) {
        this->builderData.layers = layers;
        return *this;
    }
    TextureBuilder& format(const Texture::Format format) {
        this->builderData.format = format;
        return *this;
    }

    TextureBuilder& internalFormat(const Texture::InternalFormat internalFormat) {
        this->builderData.internalFormat = internalFormat;
        return *this;
    }

    TextureBuilder& type(const Texture::TextureType type) {
        this->builderData.type = type;
        return *this;
    }

    TextureBuilder& data(const void* data) {
        this->builderData.data = data;
        return *this;
    }

    TextureBuilder& fromImage(const Ref<Image>& image);

    Ref<Texture> build(const Ref<Renderer>& renderer) const;

private:
    friend class Texture;
    TextureBuilder() = default;

    struct BuilderData {
        unsigned int width = 0;
        unsigned int height = 0;
        unsigned int layers = 1;
        Texture::Format format = Texture::Format::RGBA;
        Texture::InternalFormat internalFormat = Texture::InternalFormat::RGBA8;
        Texture::TextureType type = Texture::TextureType::TEXTURE_2D;
        const void* data = nullptr;
    } builderData;
};
