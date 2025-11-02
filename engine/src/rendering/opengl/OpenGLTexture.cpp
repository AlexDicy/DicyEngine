#include "pch/enginepch.h"
#include "OpenGLTexture.h"

#include "OpenGLRenderer.h"
#include "OpenGLTypes.h"

OpenGLTexture::OpenGLTexture(const TextureParams& params, const Ref<Renderer>& renderer) : Texture(params, renderer) {
    glFormat = OpenGLTypes::getFromTextureFormat(params.format);
    glInternalFormat = OpenGLTypes::getFromTextureInternalFormat(params.internalFormat);
    glTextureType = OpenGLTypes::getFromTextureType(params.type, params.samples);
    dataType = OpenGLTypes::getPixelTypeFromInternalFormat(params.internalFormat);
}

OpenGLTexture::OpenGLTexture(const GLuint id, const TextureParams& params, const Ref<Renderer>& renderer) : OpenGLTexture(params, renderer) {
    this->id = id;
}

void OpenGLTexture::setRawData(const void* data, const unsigned int size) {
    DE_ASSERT(params.width * params.height * params.internalFormat.getSize() >= size, "Buffer size is too large: {0}", size)
    DE_ASSERT(params.type == TextureType::TEXTURE_2D, "setRawData is only supported for 2D textures")

    if (!pbo) {
        this->initializePBO();
    }

    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo);
    glBufferData(GL_PIXEL_UNPACK_BUFFER, size, nullptr, GL_STREAM_DRAW);
    // map the buffer to get a pointer to its memory
    if (void* ptr = glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY)) {
        std::memcpy(ptr, data, size);
        glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
    }
    glBindTexture(GL_TEXTURE_2D, this->id);
    // update the texture from the PBO. This will be asynchronous until the texture is used
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, static_cast<int>(params.width), static_cast<int>(params.height), glFormat, dataType, nullptr);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
}

void OpenGLTexture::initializePBO() {
    glGenBuffers(1, &pbo);
    const unsigned int size = params.width * params.height * 4;
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo);
    glBufferData(GL_PIXEL_UNPACK_BUFFER, size, nullptr, GL_STREAM_DRAW);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
}
