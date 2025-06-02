#include "pch/enginepch.h"
#include "OpenGLRenderPassFramebuffer.h"

#include "glad/gl.h"
#include "rendering/opengl/OpenGLTexture2D.h"


OpenGLRenderPassFramebuffer::OpenGLRenderPassFramebuffer(const unsigned int width, const unsigned int height) : RenderPassFramebuffer(width, height) {
    unsigned int textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, static_cast<int>(width), static_cast<int>(height), 0, GL_RGBA, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    this->texture = std::make_shared<OpenGLTexture2D>(textureId, width, height, GL_RGBA32F, GL_RGBA, GL_FLOAT);
    // framebuffer
    glGenFramebuffers(1, &this->id);
    glBindFramebuffer(GL_FRAMEBUFFER, this->id);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

OpenGLRenderPassFramebuffer::~OpenGLRenderPassFramebuffer() {
    glDeleteFramebuffers(1, &this->id);
}

void OpenGLRenderPassFramebuffer::bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, this->id);
}

void OpenGLRenderPassFramebuffer::clear() const {
    this->bind();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}
