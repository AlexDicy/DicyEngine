#include "pch/enginepch.h"
#include "OpenGLTextureCube.h"

#include "OpenGLTexture.h"

#include <glad/gl.h>


void setupRenderBuffer(uint32_t& captureFramebuffer, uint32_t& captureRenderbuffer, const uint32_t size) {
    glGenFramebuffers(1, &captureFramebuffer);
    glGenRenderbuffers(1, &captureRenderbuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, captureFramebuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, size, size);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRenderbuffer);
}

void renderToCubemap(const Ref<Renderer>& renderer, const Ref<Shader>& convertShader, const Ref<Texture>& texture, const unsigned int cubeMapId, const int mipLevel = 0) {
    const auto projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    const SkyboxCube cube(renderer, convertShader, texture);
    for (unsigned int i = 0; i < 6; i++) {
        convertShader->uploadUniformMat4("uViewProjection", projection * TextureCubeUtils::invertedViewMatrices[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cubeMapId, mipLevel);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        cube.getVertexArray()->bind();
        glDrawElements(GL_TRIANGLES, static_cast<int>(cube.getVertexArray()->getIndexBuffer()->getCount()), GL_UNSIGNED_INT, nullptr);
    }
}

Ref<Texture> OpenGLTextureCube::createFromHDR(const Ref<Renderer>& renderer, const Ref<Texture>& hdrTexture, const Ref<Shader>& convertShader, const uint32_t size) {
    uint32_t captureFramebuffer;
    uint32_t captureRenderbuffer;
    setupRenderBuffer(captureFramebuffer, captureRenderbuffer, size);

    const Ref<Texture> cubeMap = Texture::builder()
                                     .size(size)
                                     .layers(6)
                                     .format(hdrTexture->getFormat())
                                     .internalFormat(hdrTexture->getInternalFormat())
                                     .type(TextureType::TEXTURE_CUBE)
                                     .filter(TextureFilter::LINEAR)
                                     .wrap(TextureWrap::CLAMP_TO_EDGE)
                                     .build(renderer);
    const unsigned int cubeMapId = std::static_pointer_cast<OpenGLTexture>(cubeMap)->getId();

    convertShader->bind();
    hdrTexture->bind(0);
    convertShader->uploadUniformInt("uEquirectangularMap", 0);

    int previousViewport[4];
    glGetIntegerv(GL_VIEWPORT, previousViewport);
    glViewport(0, 0, size, size);
    renderToCubemap(renderer, convertShader, hdrTexture, cubeMapId);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(previousViewport[0], previousViewport[1], previousViewport[2], previousViewport[3]);
    glDeleteFramebuffers(1, &captureFramebuffer);
    glDeleteRenderbuffers(1, &captureRenderbuffer);
    return cubeMap;
}

Ref<Texture> OpenGLTextureCube::createPrefilteredCubemap(const Ref<Renderer>& renderer, const Ref<Texture>& textureCube, const Ref<Shader>& convertShader, const uint32_t size) {
    uint32_t captureFramebuffer;
    uint32_t captureRenderbuffer;
    setupRenderBuffer(captureFramebuffer, captureRenderbuffer, size);

    Ref<Texture> prefilteredCubemap = Texture::builder()
                                          .size(size)
                                          .layers(6)
                                          .format(TextureFormat::RGBA)
                                          .internalFormat(TextureInternalFormat::RGBA16_FLOAT)
                                          .type(TextureType::TEXTURE_CUBE)
                                          .filterMin(TextureFilter::MIPMAP_LINEAR)
                                          .wrap(TextureWrap::CLAMP_TO_EDGE)
                                          .generateMipmaps(true)
                                          .build(renderer);
    const unsigned int prefilteredCubemapId = std::static_pointer_cast<OpenGLTexture>(prefilteredCubemap)->getId();

    convertShader->bind();
    textureCube->bind(0);
    convertShader->uploadUniformInt("uCubemap", 0);

    int previousViewport[4];
    glGetIntegerv(GL_VIEWPORT, previousViewport);

    constexpr int mipLevels = 5;
    for (unsigned int level = 0; level < mipLevels; level++) {
        const int mipSize = size * std::pow(0.5, level);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipSize, mipSize);
        glViewport(0, 0, mipSize, mipSize);

        const float roughness = static_cast<float>(level) / static_cast<float>(mipLevels - 1);
        convertShader->uploadUniformFloat("uRoughness", roughness);
        renderToCubemap(renderer, convertShader, textureCube, prefilteredCubemapId, level);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(previousViewport[0], previousViewport[1], previousViewport[2], previousViewport[3]);
    glDeleteFramebuffers(1, &captureFramebuffer);
    glDeleteRenderbuffers(1, &captureRenderbuffer);
    return prefilteredCubemap;
}
