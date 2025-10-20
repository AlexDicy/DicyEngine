#pragma once
#include <glad/gl.h>

class OpenGLFramebuffer : public Framebuffer {
public:
    OpenGLFramebuffer(const FramebufferParams& params, const Ref<Renderer>& renderer);
    OpenGLFramebuffer(GLuint id, const FramebufferParams& params, const Ref<Renderer>& renderer);

    GLuint getId() const {
        return this->id;
    }

protected:
    friend class OpenGLCommands;

    GLuint id = 0;
};
