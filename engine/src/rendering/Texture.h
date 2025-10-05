#pragma once
#include "images/CubeMap.h"

class Renderer;
class TextureBuilder;

class Texture {
public:
    virtual ~Texture() = default;

    virtual void bind(unsigned int) const = 0;

    virtual void setRawData(const void* data, unsigned int size) = 0;
    virtual void resize(unsigned int width, unsigned int height) = 0;

    void setSourcePath(const std::string& path);

    static TextureBuilder builder();

    enum Format {
        RED,
        RG,
        RGB,
        RGBA,
        BGRA,
        DEPTH,
    };

    // ReSharper disable CppInconsistentNaming
    enum InternalFormat {
        R8,
        R16,
        R16F,
        R32F,
        RG8,
        RG16,
        RG16F,
        RG32F,
        RGB8,
        RGB16,
        RGB16F,
        RGB32F,
        RGBA8,
        RGBA16,
        RGBA16F,
        RGBA32F,
        D16,
        D32,
        D24S8,
    };
    // ReSharper restore CppInconsistentNaming

protected:
    Texture() = default;

    unsigned int width;
    unsigned int height;
    unsigned int layers;
    Format format;
    InternalFormat internalFormat;

    std::string sourcePath;
};


class Texture2D : public Texture {
public:
    Texture2D(const unsigned int width, const unsigned int height) : Texture(), width(width), height(height) {}

    unsigned int getWidth() const {
        return this->width;
    }

    unsigned int getHeight() const {
        return this->height;
    }

protected:
    unsigned int width;
    unsigned int height;
};


class TextureCube : public Texture {
public:
    explicit TextureCube(const unsigned int size) : Texture(), size(size) {}

    unsigned int getSize() const {
        return this->size;
    }

    virtual Ref<CubeMap> toCubemap() const = 0;

    static const glm::mat4 invertedViewMatrices[6];
    static const glm::mat4 shadowViewMatrices[6];

protected:
    unsigned int size;
};


class TextureCubeArray : public Texture {
public:
    TextureCubeArray(const unsigned int size, const unsigned int layersCount) : Texture(), size(size), layersCount(layersCount) {}

    unsigned int getSize() const {
        return this->size;
    }

    unsigned int getLayersCount() const {
        return this->layersCount;
    }

    void setLayersCount(const unsigned int layersCount) {
        this->layersCount = layersCount;
    }

protected:
    unsigned int size;
    unsigned int layersCount;
};

class TextureBuilder {
public:
    TextureBuilder& setWidth(const unsigned int width) {
        this->width = width;
        return *this;
    }

    TextureBuilder& setHeight(const unsigned int height) {
        this->height = height;
        return *this;
    }

    TextureBuilder& setLayers(const unsigned int layers) {
        this->layers = layers;
        return *this;
    }
    TextureBuilder& setFormat(const Texture::Format format) {
        this->format = format;
        return *this;
    }

    TextureBuilder& setInternalFormat(const Texture::InternalFormat internalFormat) {
        this->internalFormat = internalFormat;
        return *this;
    }

    TextureBuilder& setData(void* data, const size_t dataSize) {
        this->data = data;
        this->dataSize = dataSize;
        return *this;
    }

    TextureBuilder& setSourcePath(const std::string& path) {
        this->sourcePath = path;
        return *this;
    }

    Ref<Texture> build(const Ref<Renderer>& renderer) const;

private:
    friend class Texture;
    TextureBuilder() = default;

    unsigned int width = 0;
    unsigned int height = 0;
    unsigned int layers = 1;
    Texture::Format format = Texture::Format::RGBA;
    Texture::InternalFormat internalFormat = Texture::InternalFormat::RGBA8;
    void* data = nullptr;
    size_t dataSize = 0;
    std::string sourcePath;
};
