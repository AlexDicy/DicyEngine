#include "pch/enginepch.h"
#include "Framebuffer.h"

Framebuffer::Framebuffer(const FramebufferParams& params, const Ref<Renderer>& renderer) : params(params), renderer(renderer) {
    for (const auto& texture : params.colorAttachments) {
        DE_ASSERT(texture->getWidth() == params.width && texture->getHeight() == params.height, "All color attachment textures must have the same dimensions as the framebuffer")
        DE_ASSERT(texture->getSamples() == params.samples, "All color attachment textures must have the same sample count as the framebuffer")
    }
    if (params.depthAttachment) {
        DE_ASSERT(params.depthAttachment->getWidth() == params.width && params.depthAttachment->getHeight() == params.height,
                  "Depth attachment texture must have the same dimensions as the framebuffer")
        DE_ASSERT(params.depthAttachment->getSamples() == params.samples, "Depth attachment texture must have the same sample count as the framebuffer")
    }
}

Framebuffer::~Framebuffer() {
    getRenderer()->destroyFramebuffer(*this);
}

void Framebuffer::bind() const {
    getRenderer()->bindFramebuffer(shared_from_this());
}

Ref<Framebuffer> FramebufferBuilder::build(const Ref<Renderer>& renderer) const {
    return renderer->createFramebuffer(this->params);
}
