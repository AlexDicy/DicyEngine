#include "pch/enginepch.h"
#include "OpenGLFramebuffer.h"

#include "glad/gl.h"


OpenGLFramebuffer::OpenGLFramebuffer(const uint32_t width, const uint32_t height) : Framebuffer(width, height) {
    // color texture
    glGenTextures(1, &this->colorTextureId);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, this->colorTextureId);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA8, static_cast<int>(width), static_cast<int>(height), true);
    // depth texture
    glGenTextures(1, &this->depthTextureId);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, this->depthTextureId);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_DEPTH24_STENCIL8, static_cast<int>(width), static_cast<int>(height), true);
    // framebuffer
    glGenFramebuffers(1, &this->id);
    glBindFramebuffer(GL_FRAMEBUFFER, this->id);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, this->colorTextureId, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE, this->depthTextureId, 0);

    // final color texture (non-multisampled, for ImGui)
    glGenTextures(1, &this->finalColorTextureId);
    glBindTexture(GL_TEXTURE_2D, this->finalColorTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, static_cast<int>(width), static_cast<int>(height), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenFramebuffers(1, &this->finalBufferId);
    glBindFramebuffer(GL_FRAMEBUFFER, this->finalBufferId);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->finalColorTextureId, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

OpenGLFramebuffer::~OpenGLFramebuffer() {
    glDeleteFramebuffers(1, &this->id);
    glDeleteTextures(1, &this->colorTextureId);
    glDeleteTextures(1, &this->depthTextureId);
}

void OpenGLFramebuffer::bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, this->id);
}

void OpenGLFramebuffer::updateFinalColorTexture() const {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, this->id);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->finalBufferId);
    glBlitFramebuffer(0, 0, static_cast<int>(this->width), static_cast<int>(this->height), 0, 0, static_cast<int>(this->width), static_cast<int>(this->height), GL_COLOR_BUFFER_BIT, GL_NEAREST);
}
