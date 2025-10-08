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
        DE_ERROR("Couldn't find corresponding OpenGL data type for DataType {0}", static_cast<int>(type));
        return 0;
    }

    static constexpr GLenum getFromTextureFormat(const Texture::Format format) {
        switch (format) {
            case Texture::Format::R:
            case Texture::Format::R_INT:
                return GL_RED;
            case Texture::Format::RG:
            case Texture::Format::RG_INT:
                return GL_RG;
            case Texture::Format::RGB:
            case Texture::Format::RGB_INT:
                return GL_RGB;
            case Texture::Format::RGBA:
            case Texture::Format::RGBA_INT:
                return GL_RGBA;
            case Texture::Format::BGRA:
                return GL_BGRA;
            case Texture::Format::DEPTH:
                return GL_DEPTH_COMPONENT;
            case Texture::Format::DEPTH_STENCIL:
                return GL_DEPTH_STENCIL;
        }
        DE_ERROR("Couldn't find corresponding OpenGL format for TextureFormat {0}", static_cast<int>(format));
        return 0;
    }

    static constexpr GLint getFromTextureInternalFormat(const Texture::InternalFormat internalFormat) {
        switch (internalFormat) {
            case Texture::InternalFormat::R8:
                return GL_R8;
            case Texture::InternalFormat::R8_SNORM:
                return GL_R8_SNORM;
            case Texture::InternalFormat::R8_UINT:
                return GL_R8UI;
            case Texture::InternalFormat::R8_INT:
                return GL_R8I;
            case Texture::InternalFormat::R16:
                return GL_R16;
            case Texture::InternalFormat::R16_SNORM:
                return GL_R16_SNORM;
            case Texture::InternalFormat::R16_UINT:
                return GL_R16UI;
            case Texture::InternalFormat::R16_INT:
                return GL_R16I;
            case Texture::InternalFormat::R16_FLOAT:
                return GL_R16F;
            case Texture::InternalFormat::R32_UINT:
                return GL_R32UI;
            case Texture::InternalFormat::R32_INT:
                return GL_R32I;
            case Texture::InternalFormat::R32_FLOAT:
                return GL_R32F;
            case Texture::InternalFormat::RG8:
                return GL_RG8;
            case Texture::InternalFormat::RG8_SNORM:
                return GL_RG8_SNORM;
            case Texture::InternalFormat::RG8_UINT:
                return GL_RG8UI;
            case Texture::InternalFormat::RG8_INT:
                return GL_RG8I;
            case Texture::InternalFormat::RG16:
                return GL_RG16;
            case Texture::InternalFormat::RG16_SNORM:
                return GL_RG16_SNORM;
            case Texture::InternalFormat::RG16_UINT:
                return GL_RG16UI;
            case Texture::InternalFormat::RG16_INT:
                return GL_RG16I;
            case Texture::InternalFormat::RG16_FLOAT:
                return GL_RG16F;
            case Texture::InternalFormat::RG32_UINT:
                return GL_RG32UI;
            case Texture::InternalFormat::RG32_INT:
                return GL_RG32I;
            case Texture::InternalFormat::RG32_FLOAT:
                return GL_RG32F;
            case Texture::InternalFormat::RGB8:
                return GL_RGB8;
            case Texture::InternalFormat::RGB8_SNORM:
                return GL_RGB8_SNORM;
            case Texture::InternalFormat::RGB8_UINT:
                return GL_RGB8UI;
            case Texture::InternalFormat::RGB8_INT:
                return GL_RGB8I;
            case Texture::InternalFormat::RGBA8:
                return GL_RGBA8;
            case Texture::InternalFormat::RGBA8_SNORM:
                return GL_RGBA8_SNORM;
            case Texture::InternalFormat::RGBA8_UINT:
                return GL_RGBA8UI;
            case Texture::InternalFormat::RGBA8_INT:
                return GL_RGBA8I;
            case Texture::InternalFormat::RGBA16:
                return GL_RGBA16;
            case Texture::InternalFormat::RGBA16_SNORM:
                return GL_RGBA16_SNORM;
            case Texture::InternalFormat::RGBA16_UINT:
                return GL_RGBA16UI;
            case Texture::InternalFormat::RGBA16_INT:
                return GL_RGBA16I;
            case Texture::InternalFormat::RGBA16_FLOAT:
                return GL_RGBA16F;
            case Texture::InternalFormat::RGBA32_UINT:
                return GL_RGBA32UI;
            case Texture::InternalFormat::RGBA32_INT:
                return GL_RGBA32I;
            case Texture::InternalFormat::RGBA32_FLOAT:
                return GL_RGBA32F;
            case Texture::InternalFormat::D16:
                return GL_DEPTH_COMPONENT16;
            case Texture::InternalFormat::D24:
                return GL_DEPTH_COMPONENT24;
            case Texture::InternalFormat::D32:
                return GL_DEPTH_COMPONENT32;
            case Texture::InternalFormat::D24S8:
                return GL_DEPTH24_STENCIL8;
        }
        DE_ERROR("Couldn't find corresponding OpenGL internal format for TextureInternalFormat {0}", static_cast<int>(internalFormat));
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
        DE_ERROR("Couldn't find corresponding OpenGL texture target for TextureType {0}", static_cast<int>(type));
        return 0;
    }

    static constexpr GLenum getPixelTypeFromInternalFormat(const Texture::InternalFormat internalFormat) {
        if (internalFormat == Texture::InternalFormat::D24S8) {
            return GL_UNSIGNED_INT_24_8;
        }

        switch (internalFormat.getDataType()) {
            case Texture::DataType::BYTE:
                return GL_BYTE;
            case Texture::DataType::UBYTE:
                return GL_UNSIGNED_BYTE;
            case Texture::DataType::USHORT:
                return GL_UNSIGNED_SHORT;
            case Texture::DataType::SHORT:
                return GL_SHORT;
            case Texture::DataType::INT:
                return GL_INT;
            case Texture::DataType::UINT:
                return GL_UNSIGNED_INT;
            case Texture::DataType::HALF_FLOAT:
                return GL_HALF_FLOAT;
            case Texture::DataType::FLOAT:
                return GL_FLOAT;
        }
        DE_ERROR("Couldn't find corresponding OpenGL pixel type for TextureInternalFormat {0}", static_cast<int>(internalFormat));
        return 0;
    }
};
