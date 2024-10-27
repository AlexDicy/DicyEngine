#include "pch/enginepch.h"
#include "OpenGLDepthFramebuffer.h"

#include "glad/gl.h"


OpenGLDepthFramebuffer::OpenGLDepthFramebuffer(const uint32_t width, const uint32_t height) : DepthFramebuffer(width, height) {
    // depth texture
    glGenTextures(1, &this->depthTextureId);
    glBindTexture(GL_TEXTURE_2D, this->depthTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, static_cast<int>(width), static_cast<int>(height), 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // framebuffer
    glGenFramebuffers(1, &this->id);
    glBindFramebuffer(GL_FRAMEBUFFER, this->id);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->depthTextureId, 0);
    glDrawBuffer(GL_NONE); // we don't have a color buffer
    glReadBuffer(GL_NONE);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

OpenGLDepthFramebuffer::~OpenGLDepthFramebuffer() {
    glDeleteFramebuffers(1, &this->id);
    glDeleteTextures(1, &this->depthTextureId);
}

void OpenGLDepthFramebuffer::bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, this->id);
}
