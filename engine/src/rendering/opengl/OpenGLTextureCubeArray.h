#pragma once
#include "rendering/Texture.h"


class OpenGLTextureCubeArray : public TextureCubeArray {
public:
    explicit OpenGLTextureCubeArray(unsigned int id, unsigned int size, unsigned int layersCount);
    ~OpenGLTextureCubeArray() override;

    void bind(unsigned int) const override;
    void setRawData(const void* data, unsigned size) override {} // todo:
    void resize(unsigned width, unsigned height) override {} // todo:
    Ref<CubeMap> toCubemap() const override { return nullptr; }

private:
    unsigned int id;
};
