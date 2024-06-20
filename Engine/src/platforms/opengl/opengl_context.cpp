#include "pch/enginepch.h"
#include "opengl_context.h"
#include "glad/gl.h"
#include "GLFW/glfw3.h"

void OpenGLContext::init() {
    glfwMakeContextCurrent(this->window);

    if (!gladLoadGL(glfwGetProcAddress)) {
        DE_ERROR("Failed to initialize GLAD with OpenGL context");
    }
}

void OpenGLContext::swap_buffers() {
    glfwSwapBuffers(this->window);
}
