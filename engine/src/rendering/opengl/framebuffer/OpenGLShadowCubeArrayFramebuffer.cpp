#include "pch/enginepch.h"
#include "OpenGLShadowCubeArrayFramebuffer.h"

#include "rendering/opengl/OpenGLTexture.h"

#include <glad/gl.h>

OpenGLShadowCubeArrayFramebuffer::OpenGLShadowCubeArrayFramebuffer(const unsigned int size) : ShadowCubeArrayFramebuffer(size) {
    // depth texture
    glGenTextures(1, &this->depthTextureId);
    glBindTexture(GL_TEXTURE_2D, depthTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, static_cast<int>(size), static_cast<int>(size), 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // cube map array texture
    glGenTextures(1, &this->shadowCubeTextureId);
    glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, this->shadowCubeTextureId);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    this->shadowCubeTexture =
        std::make_shared<OpenGLTexture>(this->shadowCubeTextureId, size, size, 0, TextureFormat::R, TextureInternalFormat::R32_FLOAT, TextureType::TEXTURE_CUBE_ARRAY);
    // framebuffer
    glGenFramebuffers(1, &this->id);
    glBindFramebuffer(GL_FRAMEBUFFER, this->id);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->depthTextureId, 0);
    glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, this->shadowCubeTextureId, 0, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

OpenGLShadowCubeArrayFramebuffer::~OpenGLShadowCubeArrayFramebuffer() {
    glDeleteFramebuffers(1, &this->id);
    glDeleteTextures(1, &this->depthTextureId);
}

void OpenGLShadowCubeArrayFramebuffer::bind(const unsigned int layer, const unsigned int face) const {
    glBindFramebuffer(GL_FRAMEBUFFER, this->id);
    glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, this->shadowCubeTextureId, 0, static_cast<int>(layer * 6 + face));
}

void OpenGLShadowCubeArrayFramebuffer::ensureLayersCapacity(const unsigned int layersCount) {
    if (layersCount <= this->shadowCubeTexture->getLayers()) { // at the moment we only scale up
        return;
    }

    this->shadowCubeTexture->resize(this->size, this->size, layersCount * 6);
}
