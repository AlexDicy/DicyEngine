#pragma once

class Image {
public:
    Image() = default;
    explicit Image(const std::string& path);
    Image(unsigned int width, unsigned int height, unsigned int channels, unsigned int bytesPerPixel, const void* data);

    Image(Image&& other) noexcept : width(other.width), height(other.height), channels(other.channels), bytesPerPixel(other.bytesPerPixel), data(std::move(other.data)) {}
    Image& operator=(Image&& other) noexcept {
        if (this == &other)
            return *this;
        width = other.width;
        height = other.height;
        channels = other.channels;
        bytesPerPixel = other.bytesPerPixel;
        data = std::move(other.data);
        return *this;
    }

    virtual ~Image() = default;


    unsigned int getWidth() const {
        return width;
    }

    unsigned int getHeight() const {
        return height;
    }

    unsigned int getChannels() const {
        return channels;
    }

    unsigned int getBytesPerPixel() const {
        return bytesPerPixel;
    }

    void* getData() const {
        return data.get();
    }

    void* getPixelPointer(unsigned int x, unsigned int y) const;

protected:
    unsigned int width;
    unsigned int height;
    unsigned int channels;
    unsigned int bytesPerPixel;
    std::unique_ptr<uint8_t[]> data;
};
