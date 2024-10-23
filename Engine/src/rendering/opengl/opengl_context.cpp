#include "pch/enginepch.h"
#include "opengl_context.h"
#include "glad/gl.h"
#include "GLFW/glfw3.h"

void OpenGLContext::init() {
    glfwMakeContextCurrent(this->window);

    if (!gladLoadGL(glfwGetProcAddress)) {
        DE_ERROR("Failed to initialize GLAD with OpenGL context");
    }
    auto version = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    DE_INFO("OpenGL Renderer version: {0}", version);
}

void OpenGLContext::swapBuffers() {
    glfwSwapBuffers(this->window);
}
