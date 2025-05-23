#include "pch/enginepch.h"
#include "OpenGLTexture2D.h"

#include <glad/gl.h>
#include <stb_image.h>

OpenGLTexture2D::OpenGLTexture2D(const std::string& path) : Texture2D(0, 0), path(path) {
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

    this->createTextureWithData(channels, width, height, isHDR, texture);
    stbi_image_free(texture);
}

OpenGLTexture2D::OpenGLTexture2D(const unsigned int channels, const unsigned int width, const unsigned int height, const unsigned int bytesPerPixel, const int format,
                                 const void* data) : Texture2D(width, height) {
    this->createTextureWithData(channels, width, height, bytesPerPixel == sizeof(float) * channels, format, data);
}

OpenGLTexture2D::OpenGLTexture2D(const unsigned int channels, const unsigned int width, const unsigned int height, const unsigned int bytesPerPixel, const void* data) :
    Texture2D(width, height) {
    this->createTextureWithData(channels, width, height, bytesPerPixel == sizeof(float) * channels, data);
}

OpenGLTexture2D::OpenGLTexture2D(const unsigned int id, const unsigned int width, const unsigned int height) :
    Texture2D(width, height), id(id), internalFormat(0), format(0), type(0) {}

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

void OpenGLTexture2D::setRawData(const unsigned int newWidth, const unsigned int newHeight, const void* data) {
    if (!this->pbo) {
        this->initializePBO();
    }

    const unsigned int size = newWidth * newHeight * 4;
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, this->pbo);
    glBufferData(GL_PIXEL_UNPACK_BUFFER, size, nullptr, GL_STREAM_DRAW);
    // map the buffer to get a pointer to its memory
    if (void* ptr = glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY)) {
        std::memcpy(ptr, data, size);
        glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
    }
    if (this->width != newWidth || this->height != newHeight) {
        this->resize(newWidth, newHeight);
    }
    glBindTexture(GL_TEXTURE_2D, this->id);
    // update the texture from the PBO. This will be asynchronous until the texture is used
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, this->width, this->height, this->format, this->type, nullptr);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
}

void OpenGLTexture2D::resize(const unsigned int width, const unsigned int height) {
    this->width = width;
    this->height = height;

    glBindTexture(GL_TEXTURE_2D, this->id);
    glTexImage2D(GL_TEXTURE_2D, 0, this->internalFormat, width, height, 0, this->format, this->type, nullptr);
}

void OpenGLTexture2D::createTextureWithData(const unsigned int channels, const unsigned int width, const unsigned int height, const bool isHDR, const void* data) {
    const int format = !isHDR && channels > 3 ? GL_RGBA : GL_RGB;
    this->createTextureWithData(channels, width, height, isHDR, format, data);
}

void OpenGLTexture2D::createTextureWithData(const unsigned int channels, const unsigned int width, const unsigned int height, const bool isHDR, const int format,
                                            const void* data) {
    this->width = width;
    this->height = height;
    this->internalFormat = isHDR ? GL_RGB16F : channels > 3 ? GL_RGBA8 : GL_RGB8;
    this->format = format;
    this->type = isHDR ? GL_FLOAT : GL_UNSIGNED_BYTE;

    glGenTextures(1, &this->id);
    glBindTexture(GL_TEXTURE_2D, this->id);
    glTexImage2D(GL_TEXTURE_2D, 0, this->internalFormat, width, height, 0, this->format, this->type, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    if (isHDR) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
}

void OpenGLTexture2D::initializePBO() {
    glGenBuffers(1, &this->pbo);
    const unsigned int size = this->width * this->height * 4;
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, this->pbo);
    glBufferData(GL_PIXEL_UNPACK_BUFFER, size, nullptr, GL_STREAM_DRAW);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
}
