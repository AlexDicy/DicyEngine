#include "pch/enginepch.h"
#include "window.h"
#include "events/dispatcher.h"

#include "glad/gl.h"
#include "GLFW/glfw3.h"

namespace {
    bool is_glfw_initialized = false;
}

std::shared_ptr<Window> Window::create(const char *title, const unsigned int width, const unsigned int height) {
    return std::make_shared<WindowsWindow>(title, width, height);
}

WindowsWindow::WindowsWindow(const char *title, const unsigned int width, const unsigned int height) {
    if (!is_glfw_initialized) {
        if (!glfwInit()) {
            DE_ERROR("Failed to initialize GLFW");
            return;
        }
        glfwSetErrorCallback([](int error, const char *description) {
            DE_ERROR("GLFW error [{0}] {1}", error, description);
        });
        is_glfw_initialized = true;
    }

    this->window = glfwCreateWindow(static_cast<int>(width), static_cast<int>(height), title, nullptr, nullptr);
    glfwMakeContextCurrent(this->window);

    if (!gladLoadGL(glfwGetProcAddress)) {
        DE_ERROR("Failed to initialize GLAD with OpenGL context");
        return;
    }

    this->register_events();
}

WindowsWindow::~WindowsWindow() {
    glfwDestroyWindow(this->window);
}

void WindowsWindow::update() {
    glfwPollEvents();
    glfwSwapBuffers(this->window);
}

void WindowsWindow::destroy() {
    glfwDestroyWindow(this->window);
}

unsigned int WindowsWindow::get_width() const {
    int width, height;
    glfwGetWindowSize(this->window, &width, &height);
    return static_cast<unsigned int>(width);
}

unsigned int WindowsWindow::get_height() const {
    int width, height;
    glfwGetWindowSize(this->window, &width, &height);
    return static_cast<unsigned int>(height);
}

bool WindowsWindow::is_vsync() const {
    return this->vsync;
}

void WindowsWindow::set_vsync(const bool vsync) {
    this->vsync = vsync;
    glfwSwapInterval(this->vsync ? 1 : 0);
}

void WindowsWindow::register_events() const {
    static auto event_dispatcher = EventDispatcher::get();
    glfwSetWindowSizeCallback(this->window, [](GLFWwindow *, int new_width, int new_height) {
        event_dispatcher->dispatch(WindowResizeEvent(static_cast<unsigned int>(new_width), static_cast<unsigned int>(new_height)));
    });
    glfwSetWindowCloseCallback(this->window, [](GLFWwindow *) {
        event_dispatcher->dispatch(WindowCloseEvent());
    });
    glfwSetKeyCallback(this->window, [](GLFWwindow *, int key, int, int action, int) {
        switch (action) {
            case GLFW_PRESS:
                event_dispatcher->dispatch(KeyPressedEvent(static_cast<unsigned int>(key), 0));
                break;
            case GLFW_REPEAT:
                event_dispatcher->dispatch(KeyPressedEvent(static_cast<unsigned int>(key), 1));
                break;
            case GLFW_RELEASE:
                event_dispatcher->dispatch(KeyReleasedEvent(static_cast<unsigned int>(key)));
                break;
            default:
                break;
        }
    });
    glfwSetMouseButtonCallback(this->window, [](GLFWwindow *, int button, int action, int) {
        switch (action) {
            case GLFW_PRESS:
                event_dispatcher->dispatch(MouseButtonPressedEvent(static_cast<unsigned int>(button)));
                break;
            case GLFW_RELEASE:
                event_dispatcher->dispatch(MouseButtonReleasedEvent(static_cast<unsigned int>(button)));
                break;
            default:
                break;
        }
    });
    glfwSetScrollCallback(this->window, [](GLFWwindow *, double offset_x, double offset_y) {
        event_dispatcher->dispatch(MouseScrolledEvent(offset_x, offset_y));
    });
    glfwSetCursorPosCallback(this->window, [](GLFWwindow *, double x, double y) {
        event_dispatcher->dispatch(MouseMovedEvent(x, y));
    });
}
