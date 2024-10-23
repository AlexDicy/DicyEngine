#pragma once
#include "rendering/graphic_context.h"

struct GLFWwindow;

static bool isGLFWInitialized = false;

class OpenGLContext : public GraphicContext {
    GLFWwindow *window;

public:
    OpenGLContext(GLFWwindow *window) : window(window) {}

    void init() override;
    void swapBuffers() override;
};
