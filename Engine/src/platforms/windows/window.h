#pragma once
#include <window.h>
#include "input/inputcodes.h"

struct GLFWwindow; // do not include 'GLFW' here, it would interfere with 'glad'

class DE_API WindowsWindow : public Window {
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
    GLFWwindow *get_native_window() const override {
        return this->window;
    }

private:
    void register_events() const;
};

InputCode convert_key_to_input_code(int key);
InputCode convert_mouse_button_to_input_code(int button);
