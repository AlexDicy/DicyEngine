#include "pch/enginepch.h"
#include "opengl_framebuffer.h"

#include "glad/gl.h"


OpenGLFramebuffer::OpenGLFramebuffer(const uint32_t width, const uint32_t height) : Framebuffer(width, height) {
#ifdef OPENGL_4_6
    // color texture
    glCreateTextures(GL_TEXTURE_2D, 1, &this->colorTextureId);
    glBindTexture(GL_TEXTURE_2D, this->colorTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, static_cast<int>(width), static_cast<int>(height), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTextureParameteri(this->colorTextureId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(this->colorTextureId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // depth texture
    glCreateTextures(GL_TEXTURE_2D, 1, &this->depthTextureId);
    glBindTexture(GL_TEXTURE_2D, this->depthTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, static_cast<int>(width), static_cast<int>(height), 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
    glTextureParameteri(this->depthTextureId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(this->depthTextureId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // framebuffer
    glCreateFramebuffers(1, &this->id);
    glBindFramebuffer(GL_FRAMEBUFFER, this->id);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->colorTextureId, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, this->depthTextureId, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
#else
    // color texture
    glGenTextures(1, &this->color_texture_id);
    glBindTexture(GL_TEXTURE_2D, this->color_texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, static_cast<int>(width), static_cast<int>(height), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // depth texture
    glGenTextures(1, &this->depth_texture_id);
    glBindTexture(GL_TEXTURE_2D, this->depth_texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, static_cast<int>(width), static_cast<int>(height), 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // framebuffer
    glGenFramebuffers(1, &this->id);
    glBindFramebuffer(GL_FRAMEBUFFER, this->id);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->color_texture_id, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, this->depth_texture_id, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
#endif
}

OpenGLFramebuffer::~OpenGLFramebuffer() {
    glDeleteFramebuffers(1, &this->id);
    glDeleteTextures(1, &this->colorTextureId);
    glDeleteTextures(1, &this->depthTextureId);
}

void OpenGLFramebuffer::bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, this->id);
}
