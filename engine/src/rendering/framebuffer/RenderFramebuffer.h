#pragma once

class RenderFramebuffer {
public:
    RenderFramebuffer(const uint32_t width, const uint32_t height) : width(width), height(height) {}
    virtual ~RenderFramebuffer() = default;

    uint32_t getWidth() const {
        return this->width;
    }

    uint32_t getHeight() const {
        return this->height;
    }

    virtual void bind() const = 0;

    virtual int getMousePickingValue(int x, int y) const = 0;

    virtual void clear() const = 0;
    virtual void saveMousePicking() const = 0;

protected:
    uint32_t width;
    uint32_t height;
};
