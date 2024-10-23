#include "pch/enginepch.h"
#include "opengl_texture_2d.h"

#include <glad/gl.h>
#include <stb_image.h>

OpenGLTexture2D::OpenGLTexture2D(const std::string& path) : path(path) {
    const bool isHDR = path.ends_with(".hdr");
    int width;
    int height;
    int channels;

    stbi_set_flip_vertically_on_load(true);
    void* texture;
    if (isHDR) {
        texture = stbi_loadf(path.c_str(), &width, &height, &channels, 0);
    } else {
        texture = stbi_load(path.c_str(), &width, &height, &channels, 0);
    }

    if (!texture) {
        const char* error = stbi_failure_reason();
        DE_ERROR("Failed to read texture file {0} - {1}", path, error);
    }

    this->width = width;
    this->height = height;

    this->createTextureWithData(channels, width, height, texture, isHDR);

    stbi_image_free(texture);
}

OpenGLTexture2D::OpenGLTexture2D(const unsigned int channels, const unsigned int width, const unsigned int height, const unsigned int bytesPerPixel, const void* data) {
    this->createTextureWithData(channels, width, height, data, bytesPerPixel == sizeof(float) * channels);
}

OpenGLTexture2D::~OpenGLTexture2D() {
    glDeleteTextures(1, &this->id);
}


void OpenGLTexture2D::bind(const uint32_t slot) const {
#ifdef OPENGL_4_6
    glBindTextureUnit(slot, this->id);
#else
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, this->id);
#endif
}

void OpenGLTexture2D::createTextureWithData(unsigned int channels, unsigned int width, unsigned int height, const void* data, const bool isHDR) {
    this->width = width;
    this->height = height;
    const int internalFormat = isHDR ? GL_RGB16F : channels > 3 ? GL_RGBA8 : GL_RGB8;
    const int format = !isHDR && channels > 3 ? GL_RGBA : GL_RGB;
    const int type = isHDR ? GL_FLOAT : GL_UNSIGNED_BYTE;

#ifdef OPENGL_4_6
    glCreateTextures(GL_TEXTURE_2D, 1, &this->id);
    glTextureStorage2D(this->id, 1, internalFormat, width, height);
    glTextureParameteri(this->id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(this->id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    if (isHDR) {
        glTextureParameteri(this->id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTextureParameteri(this->id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
    glTextureSubImage2D(this->id, 0, 0, 0, width, height, format, type, data);
#else
    glGenTextures(1, &this->id);
    glBindTexture(GL_TEXTURE_2D, this->id);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    if (isHDR) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
#endif
}
