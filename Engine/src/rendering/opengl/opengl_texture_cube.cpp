#include "pch/enginepch.h"
#include "opengl_texture_cube.h"

#include <glad/gl.h>
#include <stb_image.h>


OpenGLTextureCube::OpenGLTextureCube(const std::array<std::string, 6>& paths) : paths(paths) {
    glGenTextures(1, &this->id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->id);

    int width;
    int height;
    int channels;
    for (unsigned int i = 0; i < paths.size(); i++) {
        stbi_uc* texture = stbi_load(paths[i].c_str(), &width, &height, &channels, 0);
        if (!texture) {
            const char* error = stbi_failure_reason();
            DE_ERROR("Failed to read texture file {0} - {1}", paths[i], error);
        }

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);
        stbi_image_free(texture);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

OpenGLTextureCube::~OpenGLTextureCube() {
    glDeleteTextures(1, &this->id);
}

void OpenGLTextureCube::bind(uint32_t slot) const {
#ifdef OPENGL_4_6
    glBindTextureUnit(slot, this->id);
#else
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->id);
#endif
}
