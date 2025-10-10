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

    static constexpr GLenum getFromTextureFormat(const TextureFormat format) {
        switch (format) {
            case TextureFormat::R:
                return GL_RED;
            case TextureFormat::R_INT:
                return GL_RED_INTEGER;
            case TextureFormat::RG:
                return GL_RG;
            case TextureFormat::RG_INT:
                return GL_RG_INTEGER;
            case TextureFormat::RGB:
                return GL_RGB;
            case TextureFormat::RGB_INT:
                return GL_RGB_INTEGER;
            case TextureFormat::RGBA:
                return GL_RGBA;
            case TextureFormat::RGBA_INT:
                return GL_RGBA_INTEGER;
            case TextureFormat::BGRA:
                return GL_BGRA;
            case TextureFormat::DEPTH:
                return GL_DEPTH_COMPONENT;
            case TextureFormat::DEPTH_STENCIL:
                return GL_DEPTH_STENCIL;
        }
        DE_ERROR("Couldn't find corresponding OpenGL format for TextureFormat {0}", static_cast<int>(format));
        return 0;
    }

    static constexpr GLint getFromTextureInternalFormat(const TextureInternalFormat internalFormat) {
        switch (internalFormat) {
            case TextureInternalFormat::R8:
                return GL_R8;
            case TextureInternalFormat::R8_SNORM:
                return GL_R8_SNORM;
            case TextureInternalFormat::R8_UINT:
                return GL_R8UI;
            case TextureInternalFormat::R8_INT:
                return GL_R8I;
            case TextureInternalFormat::R16:
                return GL_R16;
            case TextureInternalFormat::R16_SNORM:
                return GL_R16_SNORM;
            case TextureInternalFormat::R16_UINT:
                return GL_R16UI;
            case TextureInternalFormat::R16_INT:
                return GL_R16I;
            case TextureInternalFormat::R16_FLOAT:
                return GL_R16F;
            case TextureInternalFormat::R32_UINT:
                return GL_R32UI;
            case TextureInternalFormat::R32_INT:
                return GL_R32I;
            case TextureInternalFormat::R32_FLOAT:
                return GL_R32F;
            case TextureInternalFormat::RG8:
                return GL_RG8;
            case TextureInternalFormat::RG8_SNORM:
                return GL_RG8_SNORM;
            case TextureInternalFormat::RG8_UINT:
                return GL_RG8UI;
            case TextureInternalFormat::RG8_INT:
                return GL_RG8I;
            case TextureInternalFormat::RG16:
                return GL_RG16;
            case TextureInternalFormat::RG16_SNORM:
                return GL_RG16_SNORM;
            case TextureInternalFormat::RG16_UINT:
                return GL_RG16UI;
            case TextureInternalFormat::RG16_INT:
                return GL_RG16I;
            case TextureInternalFormat::RG16_FLOAT:
                return GL_RG16F;
            case TextureInternalFormat::RG32_UINT:
                return GL_RG32UI;
            case TextureInternalFormat::RG32_INT:
                return GL_RG32I;
            case TextureInternalFormat::RG32_FLOAT:
                return GL_RG32F;
            case TextureInternalFormat::RGB8:
                return GL_RGB8;
            case TextureInternalFormat::RGB8_SNORM:
                return GL_RGB8_SNORM;
            case TextureInternalFormat::RGB8_UINT:
                return GL_RGB8UI;
            case TextureInternalFormat::RGB8_INT:
                return GL_RGB8I;
            case TextureInternalFormat::RGBA8:
                return GL_RGBA8;
            case TextureInternalFormat::RGBA8_SNORM:
                return GL_RGBA8_SNORM;
            case TextureInternalFormat::RGBA8_UINT:
                return GL_RGBA8UI;
            case TextureInternalFormat::RGBA8_INT:
                return GL_RGBA8I;
            case TextureInternalFormat::RGBA16:
                return GL_RGBA16;
            case TextureInternalFormat::RGBA16_SNORM:
                return GL_RGBA16_SNORM;
            case TextureInternalFormat::RGBA16_UINT:
                return GL_RGBA16UI;
            case TextureInternalFormat::RGBA16_INT:
                return GL_RGBA16I;
            case TextureInternalFormat::RGBA16_FLOAT:
                return GL_RGBA16F;
            case TextureInternalFormat::RGBA32_UINT:
                return GL_RGBA32UI;
            case TextureInternalFormat::RGBA32_INT:
                return GL_RGBA32I;
            case TextureInternalFormat::RGBA32_FLOAT:
                return GL_RGBA32F;
            case TextureInternalFormat::D16:
                return GL_DEPTH_COMPONENT16;
            case TextureInternalFormat::D24:
                return GL_DEPTH_COMPONENT24;
            case TextureInternalFormat::D32:
                return GL_DEPTH_COMPONENT32;
            case TextureInternalFormat::D24S8:
                return GL_DEPTH24_STENCIL8;
        }
        DE_ERROR("Couldn't find corresponding OpenGL internal format for TextureInternalFormat {0}", static_cast<int>(internalFormat));
        return 0;
    }

    static constexpr GLenum getFromTextureType(const TextureType type) {
        switch (type) {
            case TextureType::TEXTURE_2D:
                return GL_TEXTURE_2D;
            case TextureType::TEXTURE_2D_ARRAY:
                return GL_TEXTURE_2D_ARRAY;
            case TextureType::TEXTURE_CUBE:
                return GL_TEXTURE_CUBE_MAP;
            case TextureType::TEXTURE_CUBE_ARRAY:
                return GL_TEXTURE_CUBE_MAP_ARRAY;
        }
        DE_ERROR("Couldn't find corresponding OpenGL texture target for TextureType {0}", static_cast<int>(type));
        return 0;
    }

    static constexpr GLenum getPixelTypeFromInternalFormat(const TextureInternalFormat internalFormat) {
        if (internalFormat == TextureInternalFormat::D24S8) {
            return GL_UNSIGNED_INT_24_8;
        }

        switch (internalFormat.getDataType()) {
            case TextureDataType::BYTE:
                return GL_BYTE;
            case TextureDataType::UBYTE:
                return GL_UNSIGNED_BYTE;
            case TextureDataType::USHORT:
                return GL_UNSIGNED_SHORT;
            case TextureDataType::SHORT:
                return GL_SHORT;
            case TextureDataType::INT:
                return GL_INT;
            case TextureDataType::UINT:
                return GL_UNSIGNED_INT;
            case TextureDataType::HALF_FLOAT:
                return GL_HALF_FLOAT;
            case TextureDataType::FLOAT:
                return GL_FLOAT;
        }
        DE_ERROR("Couldn't find corresponding OpenGL pixel type for TextureInternalFormat {0}", static_cast<int>(internalFormat));
        return 0;
    }
};
