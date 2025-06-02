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

private:
    unsigned int id;
    Ref<Texture2D> texture;
};
