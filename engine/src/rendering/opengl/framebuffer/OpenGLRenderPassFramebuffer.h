#pragma once
#include "rendering/framebuffer/RenderPassFramebuffer.h"

class OpenGLRenderPassFramebuffer : public RenderPassFramebuffer {
public:
    OpenGLRenderPassFramebuffer(unsigned int width, unsigned int height);
    ~OpenGLRenderPassFramebuffer() override;

    void bind() const override;
    void clear() const override;

    const Ref<Texture2D>& getTexture() const override {
        return this->texture;
    }

    const Ref<Texture2D>& getDepthTexture() const override {
        return this->depthTexture;
    }

    void copyDepthToBuffer(const Ref<RenderPassFramebuffer>& buffer) const override;

private:
    unsigned int id;
    Ref<Texture2D> texture;
    Ref<Texture2D> depthTexture;
};
