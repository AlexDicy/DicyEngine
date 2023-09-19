#pragma once
#include <window.h>
#include "GLFW/glfw3.h"

class WindowsWindow : public Window {
    GLFWwindow *window;
    bool vsync = false;

public:
    WindowsWindow(const char *title, unsigned int width, unsigned int height);
    ~WindowsWindow() override;
    void destroy() override;
    void update() override;
    unsigned get_width() const override;
    unsigned get_height() const override;
    bool is_vsync() const override;
    void set_vsync(bool vsync) override;
};
