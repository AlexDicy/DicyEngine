#pragma once
#include <window.h>
#include "input/inputcodes.h"
#include "rendering/graphic_context.h"

struct GLFWwindow; // do not include 'GLFW' here, it would interfere with 'glad'

class GLFW3Window final : public Window {
    GraphicContext *graphic_ctx;
    GLFWwindow *window;
    bool vsync = false;
    float last_time = 0.0;
    float last_frame_time = 0.0;

public:
    GLFW3Window(const char *title, unsigned int width, unsigned int height);
    void destroy() override;
    void update() override;
    unsigned get_width() const override;
    unsigned get_height() const override;
    bool is_vsync() const override;
    void set_vsync(bool vsync) override;
    float get_last_frame_time() const override;

    GLFWwindow *get_native_window() const override {
        return this->window;
    }

private:
    void register_events() const;
};

InputCode convert_key_to_input_code(int key);
InputCode convert_mouse_button_to_input_code(int button);
