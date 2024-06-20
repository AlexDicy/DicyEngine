#pragma once
#include "rendering/graphic_context.h"

struct GLFWwindow;

static bool is_glfw_initialized = false;

class OpenGLContext : public GraphicContext {
    GLFWwindow *window;

public:
    OpenGLContext(GLFWwindow *window) : window(window) {}

    void init() override;
    void swap_buffers() override;
};
