#pragma once
#include "images/CubeMap.h"

class Texture {
public:
    virtual ~Texture() = default;

    virtual void bind(unsigned int) const = 0;
};


class Texture2D : public Texture {
public:
    Texture2D(const unsigned int width, const unsigned int height) : width(width), height(height) {}

    unsigned int getWidth() const {
        return this->width;
    }

    unsigned int getHeight() const {
        return this->height;
    }

    virtual void setRawData(const void* data) = 0;
    virtual void resize(unsigned int width, unsigned int height) = 0;

protected:
    unsigned int width;
    unsigned int height;
};


class TextureCube : public Texture {
public:
    explicit TextureCube(const unsigned int size) : size(size) {}

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
    TextureCubeArray(const unsigned int size, const unsigned int layersCount) : size(size), layersCount(layersCount) {}

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

enum TextureFormat {
    RGB = 0,
    RGBA = 1,
    BGRA = 2,
};
