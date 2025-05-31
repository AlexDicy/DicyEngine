#include "pch/enginepch.h"
#include "OpenGLRenderFramebuffer.h"

#include "glad/gl.h"


OpenGLRenderFramebuffer::OpenGLRenderFramebuffer(const uint32_t width, const uint32_t height) : RenderFramebuffer(width, height) {
    // color texture
    glGenTextures(1, &this->colorTextureId);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, this->colorTextureId);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA8, static_cast<int>(width), static_cast<int>(height), true);
    // mouse picking texture
    glGenTextures(1, &this->mousePickingTextureId);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, this->mousePickingTextureId);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_R32I, static_cast<int>(width), static_cast<int>(height), true);
    // depth texture
    glGenTextures(1, &this->depthTextureId);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, this->depthTextureId);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_DEPTH24_STENCIL8, static_cast<int>(width), static_cast<int>(height), true);
    // framebuffer
    glGenFramebuffers(1, &this->id);
    glBindFramebuffer(GL_FRAMEBUFFER, this->id);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, this->colorTextureId, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D_MULTISAMPLE, this->mousePickingTextureId, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE, this->depthTextureId, 0);
    constexpr GLenum drawBuffers[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
    glDrawBuffers(2, drawBuffers);

    // mouse picking, non-multisampled
    glGenTextures(1, &this->finalMousePickingTextureId);
    glBindTexture(GL_TEXTURE_2D, this->finalMousePickingTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32I, static_cast<int>(width), static_cast<int>(height), 0, GL_RED_INTEGER, GL_INT, nullptr);
    // mouse picking framebuffer
    glGenFramebuffers(1, &this->mousePickingBufferId);
    glBindFramebuffer(GL_FRAMEBUFFER, this->mousePickingBufferId);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->finalMousePickingTextureId, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        throw std::runtime_error("OpenGLRenderFramebuffer: Framebuffer is not complete!");
    }
    if (glGetError() != GL_NO_ERROR) {
        throw std::runtime_error("OpenGLRenderFramebuffer: OpenGL error occurred while creating framebuffer!");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

OpenGLRenderFramebuffer::~OpenGLRenderFramebuffer() {
    glDeleteFramebuffers(1, &this->id);
    glDeleteTextures(1, &this->colorTextureId);
    glDeleteTextures(1, &this->mousePickingTextureId);
    glDeleteTextures(1, &this->depthTextureId);
    glDeleteFramebuffers(1, &this->mousePickingBufferId);
    glDeleteTextures(1, &this->mousePickingTextureId);
}

void OpenGLRenderFramebuffer::bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, this->id);
}

int OpenGLRenderFramebuffer::getMousePickingValue(const int x, const int y) const {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, this->mousePickingBufferId);
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    int value = -1;
    glReadPixels(x, static_cast<int>(this->height) - y, 1, 1, GL_RED_INTEGER, GL_INT, &value);
    return value;
}

void OpenGLRenderFramebuffer::copyToBuffer(const uint32_t destinationId, const GLenum sourceAttachment) const {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, this->id);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, destinationId);
    glReadBuffer(sourceAttachment);
    if (destinationId != 0) {
        glDrawBuffer(GL_COLOR_ATTACHMENT0);
    }
    glBlitFramebuffer(0, 0, static_cast<int>(this->width), static_cast<int>(this->height), 0, 0, static_cast<int>(this->width), static_cast<int>(this->height), GL_COLOR_BUFFER_BIT,
                      GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, this->id);
}

void OpenGLRenderFramebuffer::updateMousePicking() const {
    this->copyToBuffer(this->mousePickingBufferId, GL_COLOR_ATTACHMENT1);
}

void OpenGLRenderFramebuffer::clear() const {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, this->mousePickingTextureId);
    constexpr int pickingId = -1;
    glClearTexImage(this->mousePickingTextureId, 0, GL_RED_INTEGER, GL_INT, &pickingId);
}
