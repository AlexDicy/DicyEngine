#pragma once
#include "rendering/framebuffer/RenderFramebuffer.h"

class OpenGLRenderFramebuffer : public RenderFramebuffer {
public:
    OpenGLRenderFramebuffer(const Ref<Renderer>& renderer, unsigned int width, unsigned int height);
    ~OpenGLRenderFramebuffer() override;

    void bind() const override;

    const Ref<Texture>& getDepthTexture() const override {
        return depthTexture;
    }

    int getMousePickingValue(int x, int y) const override;

    void clear() const override;
    void saveMousePicking() const override;

    void copyColorToBuffer(unsigned int destinationId, unsigned int sourceAttachmentIndex) const override;

private:
    // in-progress framebuffer, multisampled
    unsigned int id;
    Ref<Texture> colorTexture;
    Ref<Texture> mousePickingTexture;
    Ref<Texture> depthTexture;

    // rendered, non-multisampled for pixel reading
    unsigned int renderedBufferId;
    Ref<Texture> renderedMousePickingTexture;
};
