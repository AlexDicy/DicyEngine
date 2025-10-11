#include "pch/enginepch.h"
#include "OpenGLRenderPassFramebuffer.h"

#include "glad/gl.h"
#include "rendering/opengl/OpenGLTexture.h"


OpenGLRenderPassFramebuffer::OpenGLRenderPassFramebuffer(const Ref<const Renderer>& renderer, const unsigned int width, const unsigned int height) :
    RenderPassFramebuffer(width, height) {
    texture = Texture::builder().size(width, height).samples(4).format(TextureFormat::RGBA).internalFormat(TextureInternalFormat::RGBA32_FLOAT).build(renderer);
    depthTexture = Texture::builder().size(width, height).samples(4).format(TextureFormat::DEPTH).internalFormat(TextureInternalFormat::D24).build(renderer);
    const unsigned int textureId = std::static_pointer_cast<OpenGLTexture>(texture)->getId();
    const unsigned int depthTextureId = std::static_pointer_cast<OpenGLTexture>(depthTexture)->getId();
    // framebuffer
    glGenFramebuffers(1, &id);
    glBindFramebuffer(GL_FRAMEBUFFER, id);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, textureId, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE, depthTextureId, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

OpenGLRenderPassFramebuffer::~OpenGLRenderPassFramebuffer() {
    glDeleteFramebuffers(1, &id);
}

void OpenGLRenderPassFramebuffer::bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, id);
}

void OpenGLRenderPassFramebuffer::clear() const {
    bind();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRenderPassFramebuffer::copyDepthToBuffer(const Ref<RenderPassFramebuffer>& buffer) const {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, id);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, std::static_pointer_cast<OpenGLRenderPassFramebuffer>(buffer)->id);
    glBlitFramebuffer(0, 0, static_cast<int>(width), static_cast<int>(height), 0, 0, static_cast<int>(buffer->getWidth()), static_cast<int>(buffer->getHeight()),
                      GL_DEPTH_BUFFER_BIT, GL_NEAREST);
}
