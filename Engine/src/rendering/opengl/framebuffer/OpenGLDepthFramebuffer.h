#pragma once
#include "rendering/framebuffer/DepthFramebuffer.h"

class OpenGLDepthFramebuffer : public DepthFramebuffer {
public:
    OpenGLDepthFramebuffer(uint32_t width, uint32_t height);
    ~OpenGLDepthFramebuffer() override;

    void bind() const override;

    uint32_t getDepthTextureId() const override {
        return this->depthTextureId;
    }

private:
    uint32_t id;
    uint32_t depthTextureId;
};
