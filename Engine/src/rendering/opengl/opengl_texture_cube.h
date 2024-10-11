#pragma once
#include "rendering/texture.h"


class OpenGLTextureCube : public TextureCube {
public:
    explicit OpenGLTextureCube(const std::array<std::string, 6>& paths);
    ~OpenGLTextureCube() override;

    void bind(uint32_t) const override;

    uint32_t get_width() const override {
        return 0;
    }

    uint32_t get_height() const override {
        return 0;
    }

private:
    std::array<std::string, 6> paths;
    uint32_t id;
};
