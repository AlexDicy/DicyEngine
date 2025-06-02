#pragma once

class RenderPassFramebuffer {
public:
    RenderPassFramebuffer(const unsigned int width, const unsigned int height) : width(width), height(height) {}
    virtual ~RenderPassFramebuffer() = default;

    unsigned int getWidth() const {
        return this->width;
    }

    unsigned int getHeight() const {
        return this->height;
    }

    virtual void bind() const = 0;
    virtual void clear() const = 0;

    virtual const Ref<Texture2D>& getTexture() const = 0;
    virtual const Ref<Texture2D>& getDepthTexture() const = 0;

    virtual void copyDepthToBuffer(const Ref<RenderPassFramebuffer>& buffer) const = 0;

protected:
    unsigned int width;
    unsigned int height;
};
