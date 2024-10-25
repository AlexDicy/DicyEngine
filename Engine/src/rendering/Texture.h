#pragma once
#include "images/CubeMap.h"

class Texture {
public:
    virtual ~Texture() = default;

    virtual void bind(uint32_t) const = 0;

    virtual uint32_t getWidth() const = 0;
    virtual uint32_t getHeight() const = 0;
};

class Texture2D : public Texture {
};

class TextureCube : public Texture {
public:
    virtual uint32_t getSize() const = 0;

    virtual Ref<CubeMap> toCubemap() const = 0;
};
