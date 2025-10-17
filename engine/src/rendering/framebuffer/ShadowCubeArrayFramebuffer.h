#pragma once

/**
 * A framebuffer used for point light shadows. It uses a cube texture array to store the distance from the light to the closest object.
 * 
 * Internally, it uses a single depth texture reused for all the faces of the cube texture array.
 * Each level of the texture array is for a different point light.
 * Each face of a cube texture stores one 32-bit float value per pixel.
 */
class ShadowCubeArrayFramebuffer {
public:
    explicit ShadowCubeArrayFramebuffer(const unsigned int size) : size(size) {}
    virtual ~ShadowCubeArrayFramebuffer() = default;

    unsigned int getSize() const {
        return this->size;
    }

    virtual void bind(unsigned int layer, unsigned int face) const = 0;

    const Ref<Texture>& getShadowCubeArrayTexture() const {
        return this->shadowCubeTexture;
    }

    virtual void ensureLayersCapacity(unsigned int layersCount) = 0;

protected:
    unsigned int size;
    Ref<Texture> shadowCubeTexture;
};
