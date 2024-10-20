#pragma once

class Image {
public:
    explicit Image(const std::string& path);

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

    void* getPixel(unsigned int x, unsigned int y) const;

private:
    unsigned int width;
    unsigned int height;
    unsigned int channels;
    unsigned int bytesPerPixel;
    std::unique_ptr<uint8_t[]> data;
};
