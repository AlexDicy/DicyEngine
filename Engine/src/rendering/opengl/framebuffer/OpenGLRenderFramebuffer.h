#pragma once
#include "rendering/framebuffer/RenderFramebuffer.h"

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

    uint32_t getFinalColorTextureId() const override {
        return this->finalColorTextureId;
    }

    void updateFinalColorTexture() const override;
    void copyToBuffer(uint32_t destinationId) const;

private:
    uint32_t id;
    uint32_t colorTextureId;
    uint32_t depthTextureId;

    uint32_t finalBufferId;
    uint32_t finalColorTextureId;
};
