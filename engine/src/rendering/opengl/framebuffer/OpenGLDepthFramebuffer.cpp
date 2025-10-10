#include "pch/enginepch.h"
#include "OpenGLDepthFramebuffer.h"

#include "glad/gl.h"
#include "rendering/opengl/OpenGLTexture.h"


OpenGLDepthFramebuffer::OpenGLDepthFramebuffer(const uint32_t width, const uint32_t height) : DepthFramebuffer(width, height) {
    // depth texture
    unsigned int depthTextureId;
    glGenTextures(1, &depthTextureId);
    glBindTexture(GL_TEXTURE_2D, depthTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, static_cast<int>(width), static_cast<int>(height), 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    this->depthTexture = std::make_shared<OpenGLTexture>(depthTextureId, width, height, 1, TextureFormat::DEPTH, TextureInternalFormat::D24, TextureType::TEXTURE_2D);
    // framebuffer
    glGenFramebuffers(1, &this->id);
    glBindFramebuffer(GL_FRAMEBUFFER, this->id);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTextureId, 0);
    glDrawBuffer(GL_NONE); // we don't have a color buffer
    glReadBuffer(GL_NONE);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

OpenGLDepthFramebuffer::~OpenGLDepthFramebuffer() {
    glDeleteFramebuffers(1, &this->id);
}

void OpenGLDepthFramebuffer::bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, this->id);
}
