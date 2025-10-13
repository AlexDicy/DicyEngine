#include "pch/enginepch.h"
#include "OpenGLTexture.h"

#include "OpenGLTypes.h"
#include "images/CubeMap.h"

OpenGLTexture::OpenGLTexture(const TextureParams& params) : Texture(params) {
    glFormat = OpenGLTypes::getFromTextureFormat(params.format);
    glInternalFormat = OpenGLTypes::getFromTextureInternalFormat(params.internalFormat);
    glTextureType = OpenGLTypes::getFromTextureType(params.type, params.samples);
    dataType = OpenGLTypes::getPixelTypeFromInternalFormat(params.internalFormat);
    if (id != 0) {
        return;
    }
}

OpenGLTexture::OpenGLTexture(const GLuint id, const TextureParams& params) : OpenGLTexture(params) {
    this->id = id;
}

OpenGLTexture::~OpenGLTexture() {
    glDeleteTextures(1, &this->id);
}


void OpenGLTexture::bind(const GLuint slot) const {
    glBindTextureUnit(slot, id);
}

void OpenGLTexture::bind() const {
    glBindTexture(glTextureType, id);
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

void OpenGLTexture::resize(const unsigned int width, const unsigned int height) {
    DE_ASSERT(params.type == TextureType::TEXTURE_2D, "Resize is only supported for 2D textures")
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, glInternalFormat, static_cast<int>(width), static_cast<int>(height), 0, glFormat, dataType, nullptr);
    params.width = width;
    params.height = height;
}

void OpenGLTexture::resize(const unsigned int width, const unsigned int height, const unsigned int layers) {
    DE_ASSERT(params.type == TextureType::TEXTURE_2D_ARRAY || params.type == TextureType::TEXTURE_CUBE_ARRAY,
              "Resize with layers is only supported for 2D Array and Cube Array textures")
    DE_ASSERT(layers % 6 == 0 || params.type == TextureType::TEXTURE_2D_ARRAY, "Cube Array textures must have a multiple of 6 layers")
    glBindTexture(glTextureType, id);
    glTexImage3D(glTextureType, 0, glInternalFormat, static_cast<int>(width), static_cast<int>(height), static_cast<int>(layers), 0, glFormat, dataType, nullptr);
    params.width = width;
    params.height = height;
    params.layers = layers;
}

void OpenGLTexture::initializePBO() {
    glGenBuffers(1, &pbo);
    const unsigned int size = params.width * params.height * 4;
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo);
    glBufferData(GL_PIXEL_UNPACK_BUFFER, size, nullptr, GL_STREAM_DRAW);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
}

Ref<CubeMap> OpenGLTexture::toCubemap() const {
    DE_ASSERT(params.type == TextureType::TEXTURE_CUBE, "toCubemap is only supported for Cube textures")
    std::array<Image, 6> faces;
    glBindTexture(glTextureType, id);
    for (int i = 0; i < 6; i++) {
        faces[i] = Image(params.width, params.height, TextureFormat::RGBA, TextureInternalFormat::RGBA32_FLOAT);
        glGetTexImage(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, GL_FLOAT, faces[i].getData().get());
    }
    return std::make_shared<CubeMap>(std::move(faces));
}
