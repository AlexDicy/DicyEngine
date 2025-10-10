#include "pch/enginepch.h"
#include "OpenGLRenderPassFramebuffer.h"

#include "glad/gl.h"
#include "rendering/opengl/OpenGLTexture.h"


OpenGLRenderPassFramebuffer::OpenGLRenderPassFramebuffer(const unsigned int width, const unsigned int height) : RenderPassFramebuffer(width, height) {
    unsigned int textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureId);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA32F, static_cast<int>(width), static_cast<int>(height), GL_TRUE);
    this->texture = std::make_shared<OpenGLTexture>(textureId, width, height, 1, TextureFormat::RGBA, TextureInternalFormat::RGBA32_FLOAT, TextureType::TEXTURE_2D);
    // depth texture
    unsigned int depthTextureId;
    glGenTextures(1, &depthTextureId);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, depthTextureId);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_DEPTH_COMPONENT24, static_cast<int>(width), static_cast<int>(height), GL_TRUE);
    this->depthTexture = std::make_shared<OpenGLTexture>(depthTextureId, width, height, 1, TextureFormat::DEPTH, TextureInternalFormat::D24, TextureType::TEXTURE_2D);
    // framebuffer
    glGenFramebuffers(1, &this->id);
    glBindFramebuffer(GL_FRAMEBUFFER, this->id);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, textureId, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE, depthTextureId, 0);

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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRenderPassFramebuffer::copyDepthToBuffer(const Ref<RenderPassFramebuffer>& buffer) const {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, this->id);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, std::static_pointer_cast<OpenGLRenderPassFramebuffer>(buffer)->id);
    glBlitFramebuffer(0, 0, static_cast<int>(this->width), static_cast<int>(this->height), 0, 0, static_cast<int>(buffer->getWidth()), static_cast<int>(buffer->getHeight()),
                      GL_DEPTH_BUFFER_BIT, GL_NEAREST);
}
