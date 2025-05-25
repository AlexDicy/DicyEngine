#pragma once
#include "rendering/BufferLayout.h"
#include "glad/gl.h"

static GLenum getOpenGLTypeFromDatatype(DataType type) {
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

static GLenum getOpenGLTypeFromTextureFormat(TextureFormat format) {
    switch (format) {
        case TextureFormat::RGB:
            return GL_RGB;
        case TextureFormat::RGBA:
            return GL_RGBA;
        case TextureFormat::BGRA:
            return GL_BGRA;
    }
    DE_ERROR("Couldn't find corresponding OpenGL type for TextureFormat {0}", static_cast<int>(format));
    return 0;
}
