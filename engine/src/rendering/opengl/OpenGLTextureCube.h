#pragma once
#include "rendering/Texture.h"


class OpenGLTextureCube : public TextureCube {
public:
    explicit OpenGLTextureCube(const std::array<std::string, 6>& paths);
    explicit OpenGLTextureCube(uint32_t id, uint32_t size);
    ~OpenGLTextureCube() override;

    void bind(uint32_t) const override;

    Ref<CubeMap> toCubemap() const override;

    void setRawData(const void* data, unsigned size) override {} // todo
    void resize(unsigned width, unsigned height) override {} // todo

    static Ref<TextureCube> createFromHDR(const Ref<Renderer>& renderer, const Ref<Texture2D>& hdrTexture, const Ref<Shader>& convertShader, uint32_t size);
    static Ref<TextureCube> createPrefilteredCubemap(const Ref<Renderer>& renderer, const Ref<TextureCube>& textureCube, const Ref<Shader>& convertShader, uint32_t size);

private:
    std::array<std::string, 6> paths;
    uint32_t id;
};
