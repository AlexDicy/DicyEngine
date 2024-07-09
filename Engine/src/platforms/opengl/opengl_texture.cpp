#include "pch/enginepch.h"
#include "opengl_texture.h"

#include <glad/gl.h>
#include <stb_image.h>

OpenGLTexture2D::OpenGLTexture2D(const std::string& path) : path(path) {
    int width;
    int height;
    int channels;

    stbi_set_flip_vertically_on_load(true);
    stbi_uc* texture = stbi_load(path.c_str(), &width, &height, &channels, 0);

    this->width = width;
    this->height = height;

    glCreateTextures(GL_TEXTURE_2D, 1, &this->id);
    glTextureStorage2D(this->id, 1, channels > 3 ? GL_RGBA8 : GL_RGB8, width, height);
    glTextureParameteri(this->id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(this->id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureSubImage2D(this->id, 0, 0, 0, width, height, channels > 3 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, texture);

    stbi_image_free(texture);
}

OpenGLTexture2D::~OpenGLTexture2D() {
    glDeleteTextures(1, &this->id);
}


void OpenGLTexture2D::bind(const uint32_t slot) const {
    glBindTextureUnit(slot, this->id);
}
