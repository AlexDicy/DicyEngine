#pragma once
#include "rendering/framebuffer/DepthFramebuffer.h"

class OpenGLDepthFramebuffer : public DepthFramebuffer {
public:
    OpenGLDepthFramebuffer(uint32_t width, uint32_t height);
    ~OpenGLDepthFramebuffer() override;

    void bind() const override;

    const Ref<Texture2D>& getDepthTexture() const override {
        return this->depthTexture;
    }

private:
    uint32_t id;
    Ref<Texture2D> depthTexture;
};
