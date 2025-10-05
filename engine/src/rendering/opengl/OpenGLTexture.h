#pragma once
#include "rendering/Texture.h"

#include <glad/gl.h>

class OpenGLTexture : public Texture {
public:
    OpenGLTexture(unsigned int width, unsigned int height, unsigned int layers, Format format, InternalFormat internalFormat, const void* data = nullptr);
    OpenGLTexture(GLuint id, unsigned int width, unsigned int height, unsigned int layers, Format format, InternalFormat internalFormat);
    ~OpenGLTexture() override;

    void bind(GLuint slot) const override;

    void setRawData(const void* data, unsigned int size) override;
    void resize(unsigned int width, unsigned int height) override;

private:
    OpenGLTexture(unsigned int width, unsigned int height, Format format, InternalFormat internalFormat);

    void createTextureWithData(const void* data);
    void initializePBO();

    GLuint id = 0;
    GLenum dataType;
    GLuint pbo = 0;
};
