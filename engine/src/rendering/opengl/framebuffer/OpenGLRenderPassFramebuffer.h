#pragma once
#include "rendering/framebuffer/RenderPassFramebuffer.h"

class OpenGLRenderPassFramebuffer : public RenderPassFramebuffer {
public:
    OpenGLRenderPassFramebuffer(const Ref<Renderer>& renderer, unsigned int width, unsigned int height);
    ~OpenGLRenderPassFramebuffer() override;

    void bind() const override;
    void clear() const override;

    const Ref<Texture>& getTexture() const override {
        return this->texture;
    }

    const Ref<Texture>& getDepthTexture() const override {
        return this->depthTexture;
    }

    void copyDepthToBuffer(const Ref<RenderPassFramebuffer>& buffer) const override;

private:
    unsigned int id;
    Ref<Texture> texture;
    Ref<Texture> depthTexture;
};
