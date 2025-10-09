#pragma once
#include "rendering/Texture.h"

#include <glad/gl.h>

class OpenGLTexture : public Texture {
public:
    OpenGLTexture(unsigned int width, unsigned int height, unsigned int layers, Format format, InternalFormat internalFormat, TextureType type, const void* data = nullptr);
    OpenGLTexture(GLuint id, unsigned int width, unsigned int height, unsigned int layers, Format format, InternalFormat internalFormat, TextureType type);
    ~OpenGLTexture() override;

    void bind(GLuint slot) const override;
    void bind() const;

    void setRawData(const void* data, unsigned int size) override;
    void resize(unsigned int width, unsigned int height) override;
    void resize(unsigned int width, unsigned int height, unsigned int layers) override;

    Ref<CubeMap> toCubemap() const override;

    GLuint getId() const {
        return this->id;
    }

    GLenum getGLFormat() const {
        return this->glFormat;
    }

    GLint getGLInternalFormat() const {
        return this->glInternalFormat;
    }

    GLenum getGLTextureType() const {
        return this->glTextureType;
    }

    GLenum getDataType() const {
        return this->dataType;
    }

private:
    void createTexture();
    void uploadData(const void* data) const;
    void initializePBO();

    GLuint id = 0;
    GLenum glFormat;
    GLint glInternalFormat;
    GLenum glTextureType;
    GLenum dataType;
    GLuint pbo = 0;
};
