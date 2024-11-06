#include "pch/enginepch.h"
#include "OpenGLTextureCubeArray.h"

#include <glad/gl.h>


OpenGLTextureCubeArray::OpenGLTextureCubeArray(const unsigned int id, const unsigned int size, const unsigned int layersCount) : TextureCubeArray(size, layersCount), id(id) {}

OpenGLTextureCubeArray::~OpenGLTextureCubeArray() {
    glDeleteTextures(1, &this->id);
}

void OpenGLTextureCubeArray::bind(const unsigned int slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, this->id);
}
