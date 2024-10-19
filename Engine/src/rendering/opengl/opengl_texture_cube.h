#pragma once
#include "rendering/texture.h"


class OpenGLTextureCube : public TextureCube {
public:
    explicit OpenGLTextureCube(const std::array<std::string, 6>& paths);
    explicit OpenGLTextureCube(uint32_t id, uint32_t size);
    ~OpenGLTextureCube() override;

    void bind(uint32_t) const override;

    uint32_t get_width() const override {
        return 0;
    }

    uint32_t get_height() const override {
        return 0;
    }

    uint32_t get_size() const override {
        return this->size;
    }

    static Ref<TextureCube> create_from_hdr(const Ref<Renderer>& renderer, const Ref<Texture2D>& hdr_texture, const Ref<Shader>& convert_shader, uint32_t size);
    static Ref<TextureCube> create_irradiance_map(const Ref<Renderer>& renderer, const Ref<TextureCube>& texture_cube, const Ref<Shader>& irradiance_shader, uint32_t size);

private:
    std::array<std::string, 6> paths;
    uint32_t id;
    uint32_t size;
};
