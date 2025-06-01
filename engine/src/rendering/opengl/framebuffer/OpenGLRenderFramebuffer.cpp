#include "pch/enginepch.h"
#include "OpenGLRenderFramebuffer.h"

#include "glad/gl.h"
#include "rendering/opengl/OpenGLTexture2D.h"


OpenGLRenderFramebuffer::OpenGLRenderFramebuffer(const uint32_t width, const uint32_t height) : RenderFramebuffer(width, height) {
    // color texture
    glGenTextures(1, &this->colorTextureId);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, this->colorTextureId);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA8, static_cast<int>(width), static_cast<int>(height), true);
    // custom stencil texture
    glGenTextures(1, &this->customStencilTextureId);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, this->customStencilTextureId);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_R8I, static_cast<int>(width), static_cast<int>(height), true);
    this->customStencilTexture = std::make_shared<OpenGLTexture2D>(this->customStencilTextureId, width, height, GL_R8I, GL_RED_INTEGER, GL_BYTE);
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
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D_MULTISAMPLE, this->customStencilTextureId, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D_MULTISAMPLE, this->mousePickingTextureId, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE, this->depthTextureId, 0);
    constexpr GLenum drawBuffers[3] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
    glDrawBuffers(3, drawBuffers);

    // mouse picking, non-multisampled
    glGenTextures(1, &this->renderedMousePickingTextureId);
    glBindTexture(GL_TEXTURE_2D, this->renderedMousePickingTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32I, static_cast<int>(width), static_cast<int>(height), 0, GL_RED_INTEGER, GL_INT, nullptr);
    // rendered framebuffer, non-multisampled for reading pixel values
    glGenFramebuffers(1, &this->renderedBufferId);
    glBindFramebuffer(GL_FRAMEBUFFER, this->renderedBufferId);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->renderedMousePickingTextureId, 0);
    // check if framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        DE_ERROR("OpenGLRenderFramebuffer: Framebuffer is not complete!");
        switch (glCheckFramebufferStatus(GL_FRAMEBUFFER)) {
            case GL_FRAMEBUFFER_UNDEFINED:
                DE_ERROR("OpenGLRenderFramebuffer: Framebuffer is undefined.");
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
                DE_ERROR("OpenGLRenderFramebuffer: Framebuffer has incomplete attachment.");
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
                DE_ERROR("OpenGLRenderFramebuffer: Framebuffer is missing an attachment.");
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
                DE_ERROR("OpenGLRenderFramebuffer: Framebuffer has incomplete draw buffer.");
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
                DE_ERROR("OpenGLRenderFramebuffer: Framebuffer has incomplete read buffer.");
                break;
            case GL_FRAMEBUFFER_UNSUPPORTED:
                DE_ERROR("OpenGLRenderFramebuffer: Framebuffer format is unsupported.");
                break;
            default:
                DE_ERROR("OpenGLRenderFramebuffer: Unknown framebuffer error.");
        }
    }
    if (glGetError() != GL_NO_ERROR) {
        DE_ERROR("OpenGLRenderFramebuffer: OpenGL error occurred during framebuffer creation.");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

OpenGLRenderFramebuffer::~OpenGLRenderFramebuffer() {
    glDeleteFramebuffers(1, &this->id);
    glDeleteTextures(1, &this->colorTextureId);
    glDeleteTextures(1, &this->mousePickingTextureId);
    glDeleteTextures(1, &this->depthTextureId);
    glDeleteFramebuffers(1, &this->renderedBufferId);
    glDeleteTextures(1, &this->renderedMousePickingTextureId);
}

void OpenGLRenderFramebuffer::bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, this->id);
    constexpr GLenum drawBuffers[3] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
    glDrawBuffers(3, drawBuffers);
}

int OpenGLRenderFramebuffer::getMousePickingValue(const int x, const int y) const {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, this->renderedBufferId);
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    int value = -1;
    glReadPixels(x, static_cast<int>(this->height) - y, 1, 1, GL_RED_INTEGER, GL_INT, &value);
    return value;
}

void OpenGLRenderFramebuffer::copyColorToBuffer(const uint32_t destinationId, const GLenum sourceAttachment) const {
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

void OpenGLRenderFramebuffer::copyDepthAndStencilTextures() const {
    // copy depth texture
    // glBindFramebuffer(GL_READ_FRAMEBUFFER, this->id);
    // glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->renderedBufferId);
    // glReadBuffer(GL_DEPTH_ATTACHMENT);
    // glDrawBuffer(GL_DEPTH_ATTACHMENT);
    // glBlitFramebuffer(0, 0, static_cast<int>(this->width), static_cast<int>(this->height), 0, 0, static_cast<int>(this->width), static_cast<int>(this->height), GL_DEPTH_BUFFER_BIT,
    //                   GL_NEAREST);
    // // copy stencil texture
    // glReadBuffer(GL_STENCIL_ATTACHMENT);
    // glDrawBuffer(GL_STENCIL_ATTACHMENT);
    // glBlitFramebuffer(0, 0, static_cast<int>(this->width), static_cast<int>(this->height), 0, 0, static_cast<int>(this->width), static_cast<int>(this->height),
    //                   GL_STENCIL_BUFFER_BIT, GL_NEAREST);
    // glBindFramebuffer(GL_READ_FRAMEBUFFER, this->id);
}

void OpenGLRenderFramebuffer::saveMousePicking() const {
    this->copyColorToBuffer(this->renderedBufferId, GL_COLOR_ATTACHMENT2);
}

void OpenGLRenderFramebuffer::saveDepthStencil() const {
    this->copyDepthAndStencilTextures();
    constexpr GLenum drawBuffers[3] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, drawBuffers);
}

void OpenGLRenderFramebuffer::clear() const {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, this->mousePickingTextureId);
    constexpr int pickingId = -1;
    glClearTexImage(this->mousePickingTextureId, 0, GL_RED_INTEGER, GL_INT, &pickingId);
}
