#include "pch/enginepch.h"
#include "OpenGLDataFramebuffer.h"

#include "glad/gl.h"
#include "rendering/opengl/OpenGLTexture2D.h"


OpenGLDataFramebuffer::OpenGLDataFramebuffer(const unsigned int width, const unsigned int height) : DataFramebuffer(width, height) {
    // data texture, similar to a stencil texture
    unsigned int dataTextureId;
    glGenTextures(1, &dataTextureId);
    glBindTexture(GL_TEXTURE_2D, dataTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8I, static_cast<int>(width), static_cast<int>(height), 0, GL_RED_INTEGER, GL_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    this->depthTexture = std::make_shared<OpenGLTexture2D>(dataTextureId, width, height, GL_R8I, GL_RED_INTEGER, GL_BYTE);
    // framebuffer
    glGenFramebuffers(1, &this->id);
    glBindFramebuffer(GL_FRAMEBUFFER, this->id);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, dataTextureId, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

OpenGLDataFramebuffer::~OpenGLDataFramebuffer() {
    glDeleteFramebuffers(1, &this->id);
}

void OpenGLDataFramebuffer::bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, this->id);
}

void OpenGLDataFramebuffer::clear() const {
    this->bind();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}
