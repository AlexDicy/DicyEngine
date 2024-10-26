#pragma once
#include "rendering/Texture.h"

class OpenGLTexture2D : public Texture2D {
public:
    explicit OpenGLTexture2D(const std::string& path);
    OpenGLTexture2D(unsigned int channels, unsigned int width, unsigned int height, unsigned int bytesPerPixel, const void* data);
    OpenGLTexture2D(unsigned int id, unsigned int width, unsigned int height);
    ~OpenGLTexture2D() override;

    void bind(uint32_t) const override;

    uint32_t getWidth() const override {
        return this->width;
    }

    uint32_t getHeight() const override {
        return this->height;
    }

    void createTextureWithData(unsigned int channels, unsigned int width, unsigned int height, const void* data, bool isHDR = false);

private:
    std::string path;
    uint32_t id;
    uint32_t width;
    uint32_t height;
};
