#pragma once
#include "rendering/texture.h"

class OpenGLTexture2D : public Texture2D {
public:
    explicit OpenGLTexture2D(const std::string& path);
    ~OpenGLTexture2D() override;

    void bind(uint32_t) const override;

    uint32_t get_width() const override {
        return this->width;
    }

    uint32_t get_height() const override {
        return this->height;
    }

private:
    std::string path;
    uint32_t id;
    uint32_t width;
    uint32_t height;
};
