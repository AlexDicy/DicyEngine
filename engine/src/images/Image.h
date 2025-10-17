#pragma once

/**
 * The Image class is similar to Texture but stores the image data in CPU memory.
 * It cannot be used directly for rendering.
 */
class Image {
public:
    Image() = default;
    Image(unsigned int width, unsigned int height, TextureFormat format, TextureInternalFormat internalFormat, std::unique_ptr<uint8_t[]> data);
    Image(unsigned int width, unsigned int height, TextureFormat format, TextureInternalFormat internalFormat);

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

    TextureFormat getFormat() const {
        return this->format;
    }

    TextureInternalFormat getInternalFormat() const {
        return this->internalFormat;
    }

    const std::unique_ptr<uint8_t[]>& getData() const {
        return this->data;
    }

    unsigned int getDataSize() const {
        return this->width * this->height * this->internalFormat.getSize();
    }

    void* getPixelPointer(unsigned int x, unsigned int y) const;

protected:
    unsigned int width;
    unsigned int height;
    TextureFormat format;
    TextureInternalFormat internalFormat;
    std::unique_ptr<uint8_t[]> data;
};
