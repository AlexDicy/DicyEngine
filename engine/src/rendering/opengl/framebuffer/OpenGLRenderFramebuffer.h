#pragma once
#include "rendering/framebuffer/RenderFramebuffer.h"

#include <glad/gl.h>

class OpenGLRenderFramebuffer : public RenderFramebuffer {
public:
    OpenGLRenderFramebuffer(uint32_t width, uint32_t height);
    ~OpenGLRenderFramebuffer() override;

    void bind() const override;

    const Ref<Texture>& getDepthTexture() const override {
        return depthTexture;
    }

    int getMousePickingValue(int x, int y) const override;

    void clear() const override;
    void saveMousePicking() const override;

    void copyColorToBuffer(uint32_t destinationId, unsigned int sourceAttachmentIndex) const override;

private:
    // in-progress framebuffer, multisampled
    uint32_t id;
    uint32_t colorTextureId;
    uint32_t mousePickingTextureId;
    uint32_t depthStencilTextureId;
    Ref<Texture> depthTexture;

    // rendered, non-multisampled for pixel reading
    uint32_t renderedBufferId;
    uint32_t renderedMousePickingTextureId;
};
