#pragma once
#include "rendering/BufferLayout.h"
#include "glad/gl.h"

class OpenGLDataType {
public:
    static constexpr GLenum getFromDataType(DataType type) {
        switch (type) {
            case DataType::BOOL:
                return GL_BOOL;
            case DataType::INT:
            case DataType::INT2:
            case DataType::INT3:
            case DataType::INT4:
                return GL_INT;
            case DataType::FLOAT:
            case DataType::FLOAT2:
            case DataType::FLOAT3:
            case DataType::FLOAT4:
                return GL_FLOAT;
        }
        DE_ERROR("Couldn't find corresponding OpenGL type for DataType {0}", static_cast<int>(type));
        return 0;
    }

    static constexpr GLenum getFromTextureFormat(const Texture::Format format) {
        switch (format) {
            case Texture::Format::RED:
                return GL_RED;
            case Texture::Format::RG:
                return GL_RG;
            case Texture::Format::RGB:
                return GL_RGB;
            case Texture::Format::RGBA:
                return GL_RGBA;
            case Texture::Format::BGRA:
                return GL_BGRA;
            case Texture::Format::DEPTH:
                return GL_DEPTH_COMPONENT;
        }
        DE_ERROR("Couldn't find corresponding OpenGL type for TextureFormat {0}", static_cast<int>(format));
        return 0;
    }

    static constexpr GLint getFromTextureInternalFormat(const Texture::InternalFormat internalFormat) {
        switch (internalFormat) {
            case Texture::InternalFormat::R8:
                return GL_R8;
            case Texture::InternalFormat::R16:
                return GL_R16;
            case Texture::InternalFormat::R16F:
                return GL_R16F;
            case Texture::InternalFormat::R32F:
                return GL_R32F;
            case Texture::InternalFormat::RG8:
                return GL_RG8;
            case Texture::InternalFormat::RG16:
                return GL_RG16;
            case Texture::InternalFormat::RG16F:
                return GL_RG16F;
            case Texture::InternalFormat::RG32F:
                return GL_RG32F;
            case Texture::InternalFormat::RGB8:
                return GL_RGB8;
            case Texture::InternalFormat::RGB16:
                return GL_RGB16;
            case Texture::InternalFormat::RGB16F:
                return GL_RGB16F;
            case Texture::InternalFormat::RGB32F:
                return GL_RGB32F;
            case Texture::InternalFormat::RGBA8:
                return GL_RGBA8;
            case Texture::InternalFormat::RGBA16:
                return GL_RGBA16;
            case Texture::InternalFormat::RGBA16F:
                return GL_RGBA16F;
            case Texture::InternalFormat::RGBA32F:
                return GL_RGBA32F;
            case Texture::InternalFormat::D16:
                return GL_DEPTH_COMPONENT16;
            case Texture::InternalFormat::D32:
                return GL_DEPTH_COMPONENT32;
            case Texture::InternalFormat::D24S8:
                return GL_DEPTH24_STENCIL8;
        }
        DE_ERROR("Couldn't find corresponding OpenGL type for TextureInternalFormat {0}", static_cast<int>(internalFormat));
        return 0;
    }

    static constexpr GLenum getFromTextureType(const Texture::TextureType type) {
        switch (type) {
            case Texture::TextureType::TEXTURE_2D:
                return GL_TEXTURE_2D;
            case Texture::TextureType::TEXTURE_2D_ARRAY:
                return GL_TEXTURE_2D_ARRAY;
            case Texture::TextureType::TEXTURE_CUBE:
                return GL_TEXTURE_CUBE_MAP;
            case Texture::TextureType::TEXTURE_CUBE_ARRAY:
                return GL_TEXTURE_CUBE_MAP_ARRAY;
        }
        DE_ERROR("Couldn't find corresponding OpenGL type for TextureType {0}", static_cast<int>(type));
        return 0;
    }

    static constexpr GLenum getPixelTypeFromInternalFormat(const Texture::InternalFormat internalFormat) {
        switch (internalFormat) {
            case Texture::InternalFormat::R8:
            case Texture::InternalFormat::RG8:
            case Texture::InternalFormat::RGB8:
            case Texture::InternalFormat::RGBA8:
                return GL_UNSIGNED_BYTE;
            case Texture::InternalFormat::R16:
            case Texture::InternalFormat::RG16:
            case Texture::InternalFormat::RGB16:
            case Texture::InternalFormat::RGBA16:
                return GL_UNSIGNED_SHORT;
            case Texture::InternalFormat::R16F:
            case Texture::InternalFormat::RG16F:
            case Texture::InternalFormat::RGB16F:
            case Texture::InternalFormat::RGBA16F:
                return GL_HALF_FLOAT;
            case Texture::InternalFormat::R32F:
            case Texture::InternalFormat::RG32F:
            case Texture::InternalFormat::RGB32F:
            case Texture::InternalFormat::RGBA32F:
                return GL_FLOAT;
            case Texture::InternalFormat::D16:
                return GL_UNSIGNED_SHORT;
            case Texture::InternalFormat::D32:
                return GL_UNSIGNED_INT;
            case Texture::InternalFormat::D24S8:
                return GL_UNSIGNED_INT_24_8;
        }
        DE_ERROR("Couldn't find corresponding OpenGL pixel type for TextureInternalFormat {0}", static_cast<int>(internalFormat));
        return 0;
    }
};
