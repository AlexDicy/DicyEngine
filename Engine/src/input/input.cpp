#include "pch/enginepch.h"
#include "input.h"


#include "GLFW/glfw3.h"
#include <ranges>

// actions
std::unordered_map<std::string, InputCode> Input::actions_mapping;
std::unordered_map<InputCode, std::vector<std::function<void()>>> Input::bindings_pressed;
std::unordered_map<InputCode, std::vector<std::function<void()>>> Input::bindings_released;
// axes
std::unordered_map<std::string, std::unordered_map<InputCode, double>> Input::axis_mapping;
std::unordered_map<InputCode, std::vector<std::pair<std::function<void(double)>, double>>> Input::axis_bindings;

std::shared_ptr<Window> Input::window;


void Input::init(EventDispatcher *event_dispatcher, const std::shared_ptr<Window> &window) {
    Input::window = window;
    // TODO: do not hardcode
    actions_mapping["move_forward"] = InputCode::KEY_W;
    actions_mapping["move_left"] = InputCode::KEY_A;
    actions_mapping["move_backward"] = InputCode::KEY_S;
    actions_mapping["move_right"] = InputCode::KEY_D;
    actions_mapping["jump"] = InputCode::KEY_SPACE;
    axis_mapping["look_x"] = {{InputCode::MOUSE_X, 1.0}};
    axis_mapping["look_y"] = {{InputCode::MOUSE_Y, 1.0}};
    axis_mapping["move_x"] = {{InputCode::KEY_D, 1.0}, {InputCode::KEY_A, -1.0}};
    axis_mapping["move_y"] = {{InputCode::KEY_W, 1.0}, {InputCode::KEY_S, -1.0}};

    event_dispatcher->register_global_handler<KeyPressedEvent>([](const KeyPressedEvent &event) {
        if (event.get_repeat_count() > 0) {
            return;
        }

        for (const auto &callback : bindings_pressed[event.get_key()]) {
            callback();
        }

        for (const auto &[callback, scale] : axis_bindings[event.get_key()]) {
            callback(scale);
        }
    });

    event_dispatcher->register_global_handler<KeyReleasedEvent>([](const KeyReleasedEvent &event) {
        for (const auto &callback : bindings_released[event.get_key()]) {
            callback();
        }

        for (const auto &callback : axis_bindings[event.get_key()] | std::views::keys) {
            callback(0.0f);
        }
    });

    static double last_x = 0.0;
    static double last_y = 0.0;
    event_dispatcher->register_global_handler<MouseMovedEvent>([](const MouseMovedEvent &event) {
        const double &x = event.get_x();
        const double &y = event.get_y();

        const double offset_x = x - last_x;
        const double offset_y = last_y - y; // reversed since y-coordinates go from bottom to top

        for (const auto &[callback, scale] : axis_bindings[InputCode::MOUSE_X]) {
            callback(offset_x * scale);
        }

        for (const auto &[callback, scale] : axis_bindings[InputCode::MOUSE_Y]) {
            callback(offset_y * scale);
        }

        last_x = x;
        last_y = y;
    });
}

#ifdef DE_PLATFORM_WINDOWS
bool Input::is_action_pressed(const std::string &action) {
    const auto status = glfwGetKey(window->get_native_window(), static_cast<int>(actions_mapping[action]));
    return status == GLFW_PRESS || status == GLFW_REPEAT;
}
#else
    #error "Input::is_action_pressed not implemented for this platform"
#endif
