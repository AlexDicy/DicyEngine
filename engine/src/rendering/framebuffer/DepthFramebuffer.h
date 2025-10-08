#pragma once

class DepthFramebuffer {
public:
    DepthFramebuffer(const uint32_t width, const uint32_t height) : width(width), height(height) {}
    virtual ~DepthFramebuffer() = default;

    uint32_t getWidth() const {
        return this->width;
    }

    uint32_t getHeight() const {
        return this->height;
    }

    virtual void bind() const = 0;

    virtual const Ref<Texture>& getDepthTexture() const = 0;

protected:
    uint32_t width;
    uint32_t height;
};
