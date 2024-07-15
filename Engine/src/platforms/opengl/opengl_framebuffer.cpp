#include "pch/enginepch.h"
#include "opengl_framebuffer.h"

#include "glad/gl.h"


OpenGLFramebuffer::OpenGLFramebuffer(const uint32_t width, const uint32_t height) : Framebuffer(width, height) {
    // color texture
    glCreateTextures(GL_TEXTURE_2D, 1, &this->color_texture_id);
    glBindTexture(GL_TEXTURE_2D, this->color_texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, static_cast<int>(width), static_cast<int>(height), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTextureParameteri(this->color_texture_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(this->color_texture_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // depth texture
    glCreateTextures(GL_TEXTURE_2D, 1, &this->depth_texture_id);
    glBindTexture(GL_TEXTURE_2D, this->depth_texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, static_cast<int>(width), static_cast<int>(height), 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
    glTextureParameteri(this->depth_texture_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(this->depth_texture_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // framebuffer
    glCreateFramebuffers(1, &this->id);
    glBindFramebuffer(GL_FRAMEBUFFER, this->id);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->color_texture_id, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, this->depth_texture_id, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

OpenGLFramebuffer::~OpenGLFramebuffer() {
    glDeleteFramebuffers(1, &this->id);
}

void OpenGLFramebuffer::bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, this->id);
}
