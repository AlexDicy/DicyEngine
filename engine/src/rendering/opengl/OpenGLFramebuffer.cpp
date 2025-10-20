#include "pch/enginepch.h"
#include "OpenGLFramebuffer.h"

#include "OpenGLRenderer.h"

OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferParams& params, const Ref<Renderer>& renderer) : Framebuffer(params, renderer) {}

OpenGLFramebuffer::OpenGLFramebuffer(const GLuint id, const FramebufferParams& params, const Ref<Renderer>& renderer) : Framebuffer(params, renderer), id(id) {}
