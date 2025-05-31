#pragma once
#include "rendering/framebuffer/RenderFramebuffer.h"

#include <glad/gl.h>

class OpenGLRenderFramebuffer : public RenderFramebuffer {
public:
    OpenGLRenderFramebuffer(uint32_t width, uint32_t height);
    ~OpenGLRenderFramebuffer() override;

    void bind() const override;

    uint32_t getColorTextureId() const override {
        return this->colorTextureId;
    }

    uint32_t getDepthTextureId() const override {
        return this->depthTextureId;
    }

    int getMousePickingValue(int x, int y) const override;

    void clear() const override;
    void updateMousePicking() const override;

    void copyToBuffer(uint32_t destinationId, GLenum sourceAttachment = GL_COLOR_ATTACHMENT0) const;

private:
    uint32_t id;
    uint32_t colorTextureId;
    uint32_t mousePickingTextureId;
    uint32_t depthTextureId;

    // non-multisampled for pixel reading
    uint32_t mousePickingBufferId;
    uint32_t finalMousePickingTextureId;
};
