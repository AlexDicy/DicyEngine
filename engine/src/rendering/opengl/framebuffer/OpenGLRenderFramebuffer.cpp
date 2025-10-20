#include "pch/enginepch.h"
#include "OpenGLRenderFramebuffer.h"

#include "glad/gl.h"
#include "rendering/opengl/OpenGLTexture.h"


OpenGLRenderFramebuffer::OpenGLRenderFramebuffer(const Ref<Renderer>& renderer, const unsigned int width, const unsigned int height) : RenderFramebuffer(width, height) {
    colorTexture = Texture::builder().size(width, height).samples(4).format(TextureFormat::RGBA).internalFormat(TextureInternalFormat::RGBA8).build(renderer);
    mousePickingTexture = Texture::builder().size(width, height).samples(4).format(TextureFormat::R_INT).internalFormat(TextureInternalFormat::R32_INT).build(renderer);
    depthTexture = Texture::builder().size(width, height).samples(4).format(TextureFormat::DEPTH_STENCIL).internalFormat(TextureInternalFormat::D24S8).build(renderer);
    const unsigned int colorTextureId = std::static_pointer_cast<OpenGLTexture>(colorTexture)->getId();
    const unsigned int mousePickingTextureId = std::static_pointer_cast<OpenGLTexture>(mousePickingTexture)->getId();
    const unsigned int depthStencilTextureId = std::static_pointer_cast<OpenGLTexture>(depthTexture)->getId();
    // framebuffer
    glGenFramebuffers(1, &id);
    glBindFramebuffer(GL_FRAMEBUFFER, id);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, colorTextureId, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D_MULTISAMPLE, mousePickingTextureId, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE, depthStencilTextureId, 0);
    constexpr GLenum drawBuffers[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
    glDrawBuffers(2, drawBuffers);

    // mouse picking, non-multisampled
    renderedMousePickingTexture = Texture::builder().size(width, height).format(TextureFormat::R_INT).internalFormat(TextureInternalFormat::R32_INT).filter(TextureFilter::NEAREST).build(renderer);
    const unsigned int renderedMousePickingTextureId = std::static_pointer_cast<OpenGLTexture>(renderedMousePickingTexture)->getId();
    // rendered framebuffer, non-multisampled for reading pixel values
    glGenFramebuffers(1, &renderedBufferId);
    glBindFramebuffer(GL_FRAMEBUFFER, renderedBufferId);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderedMousePickingTextureId, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

OpenGLRenderFramebuffer::~OpenGLRenderFramebuffer() {
    glDeleteFramebuffers(1, &id);
    glDeleteFramebuffers(1, &renderedBufferId);
}

void OpenGLRenderFramebuffer::bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, id);
}

int OpenGLRenderFramebuffer::getMousePickingValue(const int x, const int y) const {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, renderedBufferId);
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    int value = -1;
    glReadPixels(x, static_cast<int>(height) - y, 1, 1, GL_RED_INTEGER, GL_INT, &value);
    return value;
}

void OpenGLRenderFramebuffer::copyColorToBuffer(const unsigned int destinationId, const unsigned int sourceAttachmentIndex = 0) const {
    const GLenum sourceAttachment = GL_COLOR_ATTACHMENT0 + sourceAttachmentIndex;
    glBindFramebuffer(GL_READ_FRAMEBUFFER, id);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, destinationId);
    glReadBuffer(sourceAttachment);
    if (destinationId != 0) {
        glDrawBuffer(GL_COLOR_ATTACHMENT0);
    }
    glBlitFramebuffer(0, 0, static_cast<int>(width), static_cast<int>(height), 0, 0, static_cast<int>(width), static_cast<int>(height), GL_COLOR_BUFFER_BIT,
                      GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, id);
}

void OpenGLRenderFramebuffer::saveMousePicking() const {
    copyColorToBuffer(renderedBufferId, 1);
}

void OpenGLRenderFramebuffer::clear() const {
    const unsigned int mousePickingTextureId = std::static_pointer_cast<OpenGLTexture>(mousePickingTexture)->getId();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, mousePickingTextureId);
    constexpr int pickingId = -1;
    glClearTexImage(mousePickingTextureId, 0, GL_RED_INTEGER, GL_INT, &pickingId);
}
