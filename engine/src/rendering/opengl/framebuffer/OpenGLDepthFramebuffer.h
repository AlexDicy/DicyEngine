#pragma once
#include "rendering/framebuffer/DepthFramebuffer.h"

class OpenGLDepthFramebuffer : public DepthFramebuffer {
public:
    OpenGLDepthFramebuffer(const Ref<const Renderer>& renderer, unsigned int width, unsigned int height);
    ~OpenGLDepthFramebuffer() override;

    void bind() const override;

    const Ref<Texture>& getDepthTexture() const override {
        return this->depthTexture;
    }

private:
    uint32_t id;
    Ref<Texture> depthTexture;
};
