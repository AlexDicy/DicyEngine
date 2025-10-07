#pragma once
#include "rendering/Texture.h"

/**
 * The Image class is similar to Texture but stores the image data in CPU memory.
 * It cannot be used directly for rendering.
 */
class Image {
public:
    using Format = TextureFormats::Format;
    using InternalFormat = TextureFormats::InternalFormat;

    Image() = default;
    Image(unsigned int width, unsigned int height, Format format, InternalFormat internalFormat, const void* data = nullptr);

    Image(Image&& other) noexcept : width(other.width), height(other.height), format(other.format), internalFormat(other.internalFormat), data(std::move(other.data)) {}
    Image& operator=(Image&& other) noexcept {
        if (this == &other)
            return *this;
        this->width = other.width;
        this->height = other.height;
        this->format = other.format;
        this->internalFormat = other.internalFormat;
        this->data = std::move(other.data);
        return *this;
    }

    virtual ~Image() = default;


    unsigned int getWidth() const {
        return this->width;
    }

    unsigned int getHeight() const {
        return this->height;
    }

    Texture::Format getFormat() const {
        return this->format;
    }

    Texture::InternalFormat getInternalFormat() const {
        return this->internalFormat;
    }

    void* getData() const {
        return this->data.get();
    }

    unsigned int getDataSize() const {
        return this->width * this->height * this->internalFormat.getSize();
    }

    void* getPixelPointer(unsigned int x, unsigned int y) const;

protected:
    unsigned int width;
    unsigned int height;
    Format format;
    InternalFormat internalFormat;
    std::unique_ptr<uint8_t[]> data;
};
