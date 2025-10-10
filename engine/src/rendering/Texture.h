#pragma once
#include "TextureFormats.h"
#include "images/Image.h"


class CubeMap;
class Renderer;
class TextureBuilder;

class Texture {
public:
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

    TextureFormat getFormat() const {
        return this->format;
    }

    TextureInternalFormat getInternalFormat() const {
        return this->internalFormat;
    }

    TextureType getType() const {
        return this->type;
    }

    virtual void bind(unsigned int slot) const = 0;

    virtual void setRawData(const void* data, unsigned int size) = 0;
    virtual void resize(unsigned int width, unsigned int height) = 0;
    virtual void resize(unsigned int width, unsigned int height, unsigned int layers) = 0;

    virtual Ref<CubeMap> toCubemap() const = 0;

    static TextureBuilder builder();

protected:
    Texture(unsigned int width, unsigned int height, unsigned int layers, TextureFormat format, TextureInternalFormat internalFormat, TextureType type);

    unsigned int width;
    unsigned int height;
    unsigned int layers;
    TextureFormat format;
    TextureInternalFormat internalFormat;
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

    TextureBuilder& size(const unsigned int size) {
        this->width(size);
        this->height(size);
        return *this;
    }

    TextureBuilder& size(const unsigned int width, const unsigned int height) {
        this->width(width);
        this->height(height);
        return *this;
    }

    TextureBuilder& layers(const unsigned int layers) {
        this->builderData.layers = layers;
        return *this;
    }
    TextureBuilder& format(const TextureFormat format) {
        this->builderData.format = format;
        return *this;
    }

    TextureBuilder& internalFormat(const TextureInternalFormat internalFormat) {
        this->builderData.internalFormat = internalFormat;
        return *this;
    }

    TextureBuilder& type(const TextureType type) {
        this->builderData.type = type;
        return *this;
    }

    TextureBuilder& data(const void* data) {
        this->builderData.data = data;
        return *this;
    }

    TextureBuilder& fromImage(const Ref<Image>& image);

    Ref<Texture> build(const Ref<const Renderer>& renderer) const;

private:
    friend class Texture;
    TextureBuilder() = default;

    struct BuilderData {
        unsigned int width = 0;
        unsigned int height = 0;
        unsigned int layers = 1;
        TextureFormat format = TextureFormat::RGBA;
        TextureInternalFormat internalFormat = TextureInternalFormat::RGBA8;
        TextureType type = TextureType::TEXTURE_2D;
        const void* data = nullptr;
    } builderData;
};
