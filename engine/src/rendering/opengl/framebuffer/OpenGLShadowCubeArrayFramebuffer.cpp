#include "pch/enginepch.h"
#include "OpenGLShadowCubeArrayFramebuffer.h"

#include "rendering/opengl/OpenGLTexture.h"

#include <glad/gl.h>

OpenGLShadowCubeArrayFramebuffer::OpenGLShadowCubeArrayFramebuffer(const Ref<const Renderer>& renderer, const unsigned int size) : ShadowCubeArrayFramebuffer(size) {
    depthTexture = Texture::builder()
                       .size(size)
                       .format(TextureFormat::DEPTH)
                       .internalFormat(TextureInternalFormat::D24)
                       .type(TextureType::TEXTURE_2D)
                       .wrap(TextureWrap::CLAMP_TO_EDGE)
                       .build(renderer);
    shadowCubeTexture = Texture::builder()
                            .size(size)
                            .layers(0)
                            .format(TextureFormat::R)
                            .internalFormat(TextureInternalFormat::R32_FLOAT)
                            .type(TextureType::TEXTURE_CUBE_ARRAY)
                            .wrap(TextureWrap::CLAMP_TO_EDGE)
                            .build(renderer);
    const unsigned int depthTextureId = std::static_pointer_cast<const OpenGLTexture>(depthTexture)->getId();
    const unsigned int shadowCubeTextureId = std::static_pointer_cast<const OpenGLTexture>(shadowCubeTexture)->getId();
    // framebuffer
    glGenFramebuffers(1, &id);
    glBindFramebuffer(GL_FRAMEBUFFER, id);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTextureId, 0);
    glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, shadowCubeTextureId, 0, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

OpenGLShadowCubeArrayFramebuffer::~OpenGLShadowCubeArrayFramebuffer() {
    glDeleteFramebuffers(1, &id);
}

void OpenGLShadowCubeArrayFramebuffer::bind(const unsigned int layer, const unsigned int face) const {
    glBindFramebuffer(GL_FRAMEBUFFER, id);
    const unsigned int shadowCubeTextureId = std::static_pointer_cast<const OpenGLTexture>(shadowCubeTexture)->getId();
    glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, shadowCubeTextureId, 0, static_cast<int>(layer * 6 + face));
}

void OpenGLShadowCubeArrayFramebuffer::ensureLayersCapacity(const unsigned int layersCount) {
    if (layersCount <= shadowCubeTexture->getLayers()) { // at the moment we only scale up
        return;
    }

    shadowCubeTexture->resize(size, size, layersCount * 6);
}
