#include "pch/enginepch.h"
#include "OpenGLTexture.h"

#include "OpenGLDataType.h"
#include "images/CubeMap.h"

OpenGLTexture::OpenGLTexture(const unsigned int width, const unsigned int height, const unsigned int layers, const Format format, const InternalFormat internalFormat,
                             const TextureType type, const void* data) : Texture(width, height, layers, format, internalFormat, type) {
    this->glFormat = OpenGLDataType::getFromTextureFormat(this->format);
    this->glInternalFormat = OpenGLDataType::getFromTextureInternalFormat(this->internalFormat);
    this->glTextureType = OpenGLDataType::getFromTextureType(this->type);
    this->dataType = OpenGLDataType::getPixelTypeFromInternalFormat(internalFormat);
    if (data) {
        this->createTextureWithData(data);
    }
}

OpenGLTexture::OpenGLTexture(const GLuint id, const unsigned int width, const unsigned int height, const unsigned int layers, const Format format,
                             const InternalFormat internalFormat, const TextureType type) : OpenGLTexture(width, height, layers, format, internalFormat, type) {
    this->id = id;
}

OpenGLTexture::~OpenGLTexture() {
    glDeleteTextures(1, &this->id);
}


void OpenGLTexture::bind(const GLuint slot) const {
    glBindTextureUnit(slot, this->id);
}

void OpenGLTexture::setRawData(const void* data, const unsigned int size) {
    DE_ASSERT(this->width * this->height * this->internalFormat.getSize() >= size, "Buffer size is too large: {0}", size)
    DE_ASSERT(this->type == TextureType::TEXTURE_2D, "setRawData is only supported for 2D textures")

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
    DE_ASSERT(this->type == TextureType::TEXTURE_2D, "Resize is only supported for 2D textures")
    this->width = width;
    this->height = height;

    glBindTexture(GL_TEXTURE_2D, this->id);
    glTexImage2D(GL_TEXTURE_2D, 0, this->glInternalFormat, static_cast<int>(width), static_cast<int>(height), 0, this->glFormat, this->dataType, nullptr);
}

void OpenGLTexture::createTextureWithData(const void* data) {
    glGenTextures(1, &this->id);
    glBindTexture(this->glTextureType, this->id);

    switch (this->type) {
        case TextureType::TEXTURE_2D:
            glTexImage2D(this->glTextureType, 0, this->glInternalFormat, static_cast<int>(this->width), static_cast<int>(this->height), 0, this->glFormat, this->dataType, data);
            break;
        case TextureType::TEXTURE_2D_ARRAY:
        case TextureType::TEXTURE_CUBE_ARRAY:
            glTexImage3D(this->glTextureType, 0, this->glInternalFormat, static_cast<int>(this->width), static_cast<int>(this->height), static_cast<int>(this->layers), 0,
                         this->glFormat, this->dataType, data);
            break;
        case TextureType::TEXTURE_CUBE:
            {
                const size_t faceSize = static_cast<size_t>(this->width) * this->height * this->internalFormat.getSize();
                for (int face = 0; face < 6; face++) {
                    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, this->glInternalFormat, static_cast<int>(this->width), static_cast<int>(this->height), 0, this->glFormat,
                                 this->dataType, static_cast<const uint8_t*>(data) + (face * faceSize));
                }
                break;
            }
        default:
            DE_ASSERT(false, "Unsupported texture type")
    }

    glTexParameteri(this->glTextureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(this->glTextureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (this->type == TextureType::TEXTURE_CUBE || this->type == TextureType::TEXTURE_CUBE_ARRAY) {
        glTexParameteri(this->glTextureType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(this->glTextureType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(this->glTextureType, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    } else {
        glTexParameteri(this->glTextureType, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(this->glTextureType, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }
}

void OpenGLTexture::initializePBO() {
    glGenBuffers(1, &this->pbo);
    const unsigned int size = this->width * this->height * 4;
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, this->pbo);
    glBufferData(GL_PIXEL_UNPACK_BUFFER, size, nullptr, GL_STREAM_DRAW);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
}

Ref<CubeMap> OpenGLTexture::toCubemap() const {
    DE_ASSERT(this->type == TextureType::TEXTURE_CUBE, "toCubemap is only supported for Cube textures")
    std::array<Image, 6> faces;
    glBindTexture(this->glTextureType, this->id);
    for (int i = 0; i < 6; i++) {
        faces[i] = Image(this->width, this->height, Format::RGB, InternalFormat::RGB32F);
        glGetTexImage(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, GL_FLOAT, faces[i].getData());
    }
    return std::make_shared<CubeMap>(std::move(faces));
}
