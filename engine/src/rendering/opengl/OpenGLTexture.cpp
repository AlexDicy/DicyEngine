#include "pch/enginepch.h"
#include "OpenGLTexture.h"

#include "OpenGLDataType.h"

OpenGLTexture::OpenGLTexture(const unsigned int width, const unsigned int height, const unsigned int layers, const Format format, const InternalFormat internalFormat, const void* data) :
    Texture() {
    this->width = width;
    this->height = height;
    this->layers = layers;
    this->format = format;
    this->internalFormat = internalFormat;
    this->glFormat = OpenGLDataType::getFromTextureFormat(this->format);
    this->glInternalFormat = OpenGLDataType::getFromTextureInternalFormat(this->internalFormat);
    this->dataType = OpenGLDataType::getPixelTypeFromInternalFormat(internalFormat);
    if (data) {
        this->createTextureWithData(data);
    }
}

OpenGLTexture::OpenGLTexture(const GLuint id, const unsigned int width, const unsigned int height, const unsigned int layers, const Format format, const InternalFormat internalFormat) :
    OpenGLTexture(width, height, layers, format, internalFormat) {
    this->id = id;
}

OpenGLTexture::~OpenGLTexture() {
    glDeleteTextures(1, &this->id);
}


void OpenGLTexture::bind(const GLuint slot) const {
#ifdef OPENGL_4_6
    glBindTextureUnit(slot, this->id);
#else
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, this->id);
#endif
}

void OpenGLTexture::setRawData(const void* data, const unsigned int size) {
    DE_ASSERT(this->width * this->height * 4 >= size, "Buffer size is too large: {0}", size)
    if (!this->pbo) {
        this->initializePBO();
    }

    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, this->pbo);
    glBufferData(GL_PIXEL_UNPACK_BUFFER, size, nullptr, GL_STREAM_DRAW);
    // map the buffer to get a pointer to its memory
    if (void* ptr = glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY)) {
        std::memcpy(ptr, data, size);
        glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
    }
    glBindTexture(GL_TEXTURE_2D, this->id);
    // update the texture from the PBO. This will be asynchronous until the texture is used
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, static_cast<int>(this->width), static_cast<int>(this->height), this->glFormat, this->dataType, nullptr);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
}

void OpenGLTexture::resize(const unsigned int width, const unsigned int height) {
    this->width = width;
    this->height = height;

    glBindTexture(GL_TEXTURE_2D, this->id);
    glTexImage2D(GL_TEXTURE_2D, 0, this->glInternalFormat, static_cast<int>(width), static_cast<int>(height), 0, this->glFormat, this->dataType, nullptr);
}

void OpenGLTexture::createTextureWithData(const void* data) {
    glGenTextures(1, &this->id);
    glBindTexture(GL_TEXTURE_2D, this->id);
    glTexImage2D(GL_TEXTURE_2D, 0, this->glInternalFormat, static_cast<int>(width), static_cast<int>(height), 0, this->glFormat, this->dataType, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void OpenGLTexture::initializePBO() {
    glGenBuffers(1, &this->pbo);
    const unsigned int size = this->width * this->height * 4;
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, this->pbo);
    glBufferData(GL_PIXEL_UNPACK_BUFFER, size, nullptr, GL_STREAM_DRAW);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
}
