#include "pch/enginepch.h"
#include "OpenGLDataFramebuffer.h"

#include "glad/gl.h"
#include "rendering/opengl/OpenGLTexture.h"


OpenGLDataFramebuffer::OpenGLDataFramebuffer(const Ref<Renderer>& renderer, const unsigned int width, const unsigned int height) : DataFramebuffer(width, height) {
    // data texture, similar to a stencil texture
    this->depthTexture =
        Texture::builder().size(width, height).format(TextureFormat::R_INT).internalFormat(TextureInternalFormat::R8_INT).filter(TextureFilter::NEAREST).build(renderer);
    const unsigned int dataTextureId = std::static_pointer_cast<OpenGLTexture>(this->depthTexture)->getId();
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
