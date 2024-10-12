#pragma once
#include "rendering/texture.h"

class OpenGLTexture2D : public Texture2D {
public:
    explicit OpenGLTexture2D(const std::string& path);
    explicit OpenGLTexture2D(unsigned int channels, unsigned int width, unsigned int height, const void* data);
    ~OpenGLTexture2D() override;

    void bind(uint32_t) const override;

    uint32_t get_width() const override {
        return this->width;
    }

    uint32_t get_height() const override {
        return this->height;
    }

    void create_texture_with_data(unsigned int channels, unsigned int width, unsigned int height, const void* data, bool is_hdr = false);

private:
    std::string path;
    uint32_t id;
    uint32_t width;
    uint32_t height;
};
