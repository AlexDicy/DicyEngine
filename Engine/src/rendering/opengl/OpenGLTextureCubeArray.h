#pragma once
#include "rendering/Texture.h"


class OpenGLTextureCubeArray : public TextureCubeArray {
public:
    explicit OpenGLTextureCubeArray(unsigned int id, unsigned int size, unsigned int layersCount);
    ~OpenGLTextureCubeArray() override;

    void bind(unsigned int) const override;

private:
    unsigned int id;
};
