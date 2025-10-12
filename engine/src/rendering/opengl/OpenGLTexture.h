#pragma once
#include "rendering/Texture.h"

#include <glad/gl.h>

class OpenGLTexture : public Texture {
public:
    OpenGLTexture(const TextureParams& params);
    OpenGLTexture(GLuint id, const TextureParams& params);
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

protected:
    friend class OpenGLCommands;

    void initializePBO();

    GLuint id = 0;
    GLenum glFormat;
    GLint glInternalFormat;
    GLenum glTextureType;
    GLenum dataType;
    GLuint pbo = 0;
};
