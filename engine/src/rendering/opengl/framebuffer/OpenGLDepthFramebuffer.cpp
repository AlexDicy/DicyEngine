#include "pch/enginepch.h"
#include "OpenGLDepthFramebuffer.h"

#include "glad/gl.h"
#include "rendering/opengl/OpenGLTexture.h"


OpenGLDepthFramebuffer::OpenGLDepthFramebuffer(const Ref<Renderer>& renderer, const unsigned int width, const unsigned int height) : DepthFramebuffer(width, height) {
    // depth texture
    this->depthTexture =
        Texture::builder().size(width, height).format(TextureFormat::DEPTH).internalFormat(TextureInternalFormat::D24).filter(TextureFilter::NEAREST).build(renderer);
    const unsigned int depthTextureId = std::static_pointer_cast<OpenGLTexture>(this->depthTexture)->getId();
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
