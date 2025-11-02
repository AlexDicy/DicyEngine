#pragma once
#include <glad/gl.h>

class OpenGLFramebuffer : public Framebuffer {
public:
    OpenGLFramebuffer(const FramebufferParams& params, const Ref<Renderer>& renderer) : Framebuffer(params, renderer) {}
    OpenGLFramebuffer(const GLuint id, const FramebufferParams& params, const Ref<Renderer>& renderer) : Framebuffer(params, renderer), id(id) {}

    GLuint getId() const {
        return this->id;
    }

protected:
    friend class OpenGLCommands;

    GLuint id = 0;
};
