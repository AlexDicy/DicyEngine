#pragma once
#include "rendering/Texture.h"

#include <glad/gl.h>

class OpenGLTexture : public Texture {
public:
    OpenGLTexture(const TextureParams& params, const Ref<Renderer>& renderer);
    OpenGLTexture(GLuint id, const TextureParams& params, const Ref<Renderer>& renderer);
    ~OpenGLTexture() override;

    void setRawData(const void* data, unsigned int size) override;

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
