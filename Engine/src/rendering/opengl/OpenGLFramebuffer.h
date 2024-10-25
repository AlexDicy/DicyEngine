#pragma once
#include "rendering/Framebuffer.h"

class OpenGLFramebuffer : public Framebuffer {
public:
    OpenGLFramebuffer(uint32_t width, uint32_t height);
    ~OpenGLFramebuffer() override;

    void bind() const override;

    uint32_t getColorTextureId() const override {
        return this->colorTextureId;
    }

    uint32_t getDepthTextureId() const override {
        return this->depthTextureId;
    }

private:
    uint32_t id;
    uint32_t colorTextureId;
    uint32_t depthTextureId;
};
