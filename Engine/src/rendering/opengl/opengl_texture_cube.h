#pragma once
#include "rendering/texture.h"


class OpenGLTextureCube : public TextureCube {
public:
    explicit OpenGLTextureCube(const std::array<std::string, 6>& paths);
    explicit OpenGLTextureCube(uint32_t id, uint32_t size);
    ~OpenGLTextureCube() override;

    void bind(uint32_t) const override;

    uint32_t getWidth() const override {
        return 0;
    }

    uint32_t getHeight() const override {
        return 0;
    }

    uint32_t getSize() const override {
        return this->size;
    }

    Ref<CubeMap> toCubemap() const override;

    static Ref<TextureCube> createFromHDR(const Ref<Renderer>& renderer, const Ref<Texture2D>& hdrTexture, const Ref<Shader>& convertShader, uint32_t size);
    static Ref<TextureCube> createIrradianceMap(const Ref<Renderer>& renderer, const Ref<TextureCube>& textureCube, const Ref<Shader>& irradianceShader, uint32_t size);

private:
    std::array<std::string, 6> paths;
    uint32_t id;
    uint32_t size;
};
