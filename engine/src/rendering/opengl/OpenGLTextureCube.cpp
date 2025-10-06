#include "pch/enginepch.h"
#include "OpenGLTextureCube.h"

#include "images/CubeMap.h"
#include "images/Image.h"

#include <glad/gl.h>
#include <stb_image.h>


OpenGLTextureCube::OpenGLTextureCube(const std::array<std::string, 6>& paths) : TextureCube(0), paths(paths) {
    glGenTextures(1, &this->id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->id);

    int width = -1;
    int height = -1;
    int channels = 0;
    for (unsigned int i = 0; i < paths.size(); i++) {
        stbi_uc* texture = stbi_load(paths[i].c_str(), &width, &height, &channels, 0);
        if (!texture) {
            const char* error = stbi_failure_reason();
            DE_ERROR("Failed to read texture file {0} - {1}", paths[i], error);
        }

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);
        stbi_image_free(texture);
    }

    this->size = width;

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

OpenGLTextureCube::OpenGLTextureCube(const uint32_t id, const uint32_t size) : TextureCube(size), id(id) {}

OpenGLTextureCube::~OpenGLTextureCube() {
    glDeleteTextures(1, &this->id);
}

void OpenGLTextureCube::bind(uint32_t slot) const {
#ifdef OPENGL_4_6
    glBindTextureUnit(slot, this->id);
#else
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->id);
#endif
}

void setupRenderBuffer(uint32_t& captureFramebuffer, uint32_t& captureRenderbuffer, const uint32_t size) {
    glGenFramebuffers(1, &captureFramebuffer);
    glGenRenderbuffers(1, &captureRenderbuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, captureFramebuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, size, size);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRenderbuffer);
}

void createCubeMap(uint32_t& cubeMapId, const uint32_t size, const bool hasMipmaps) {
    glGenTextures(1, &cubeMapId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapId);
    for (unsigned int i = 0; i < 6; i++) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, size, size, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, hasMipmaps ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    if (hasMipmaps) {
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    }
}

void renderToCubemap(const Ref<Renderer>& renderer, const Ref<Shader>& convertShader, const Ref<Texture>& texture, const uint32_t cubeMapId, const int mipLevel = 0) {
    const auto projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    const SkyboxCube cube(renderer, convertShader, texture);
    for (unsigned int i = 0; i < 6; i++) {
        convertShader->uploadUniformMat4("uViewProjection", projection * TextureCube::invertedViewMatrices[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cubeMapId, mipLevel);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        cube.getVertexArray()->bind();
        glDrawElements(GL_TRIANGLES, static_cast<int>(cube.getVertexArray()->getIndexBuffer()->getCount()), GL_UNSIGNED_INT, nullptr);
    }
}

Ref<CubeMap> OpenGLTextureCube::toCubemap() const {
    std::array<Image, 6> faces;
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->id);
    for (int i = 0; i < 6; i++) {
        faces[i] = Image(this->size, this->size, Format::RGB, InternalFormat::RGB32F);
        glGetTexImage(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, GL_FLOAT, faces[i].getData());
    }
    return std::make_shared<CubeMap>(std::move(faces));
}

Ref<TextureCube> OpenGLTextureCube::createFromHDR(const Ref<Renderer>& renderer, const Ref<Texture>& hdrTexture, const Ref<Shader>& convertShader, const uint32_t size) {
    uint32_t captureFramebuffer;
    uint32_t captureRenderbuffer;
    setupRenderBuffer(captureFramebuffer, captureRenderbuffer, size);

    uint32_t cubeMapId;
    createCubeMap(cubeMapId, size, false);

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
    return std::make_shared<OpenGLTextureCube>(cubeMapId, size);
}

Ref<TextureCube> OpenGLTextureCube::createPrefilteredCubemap(const Ref<Renderer>& renderer, const Ref<TextureCube>& textureCube, const Ref<Shader>& convertShader,
                                                             const uint32_t size) {
    uint32_t captureFramebuffer;
    uint32_t captureRenderbuffer;
    setupRenderBuffer(captureFramebuffer, captureRenderbuffer, size);

    uint32_t prefilteredCubemapId;
    createCubeMap(prefilteredCubemapId, size, true);

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
    return std::make_shared<OpenGLTextureCube>(prefilteredCubemapId, size);
}
