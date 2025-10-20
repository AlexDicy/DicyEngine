#pragma once
#include "rendering/Texture.h"


class OpenGLTextureCube {
public:
    static Ref<Texture> createFromHDR(const Ref<Renderer>& renderer, const Ref<Texture>& hdrTexture, const Ref<Shader>& convertShader, uint32_t size);
    static Ref<Texture> createPrefilteredCubemap(const Ref<Renderer>& renderer, const Ref<Texture>& textureCube, const Ref<Shader>& convertShader, uint32_t size);
};
