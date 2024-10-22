#include "pch/enginepch.h"
#include "input.h"


#include "GLFW/glfw3.h"
#include <ranges>

// actions
std::unordered_map<std::string, InputCode> Input::actions_mapping;
std::unordered_map<InputCode, std::vector<std::function<void()>>> Input::bindings_pressed;
std::unordered_map<InputCode, std::vector<std::function<void()>>> Input::bindings_released;
// axes
std::unordered_map<std::string, std::unordered_map<InputCode, float>> Input::axis_mapping;
std::unordered_map<InputCode, std::vector<std::pair<std::function<void(float)>, float>>> Input::axis_bindings;

Ref<Window> Input::window;


void Input::init(const Ref<EventDispatcher>& event_dispatcher, const Ref<Window>& window) {
    Input::window = window;
    // TODO: do not hardcode
    actions_mapping["left_click"] = InputCode::MOUSE_BUTTON_LEFT;
    actions_mapping["right_click"] = InputCode::MOUSE_BUTTON_RIGHT;
    actions_mapping["move_forward"] = InputCode::KEY_W;
    actions_mapping["move_left"] = InputCode::KEY_A;
    actions_mapping["move_backward"] = InputCode::KEY_S;
    actions_mapping["move_right"] = InputCode::KEY_D;
    actions_mapping["jump"] = InputCode::KEY_SPACE;
    axis_mapping["look_x"] = {{InputCode::MOUSE_X, 1.0}};
    axis_mapping["look_y"] = {{InputCode::MOUSE_Y, 1.0}};
    axis_mapping["move_x"] = {{InputCode::KEY_D, 1.0}, {InputCode::KEY_A, -1.0}};
    axis_mapping["move_y"] = {{InputCode::KEY_W, 1.0}, {InputCode::KEY_S, -1.0}};

    event_dispatcher->register_global_handler<KeyPressedEvent>([](const KeyPressedEvent& event) {
        if (event.get_repeat_count() > 0) {
            return;
        }

        for (const auto& callback : bindings_pressed[event.get_key()]) {
            callback();
        }

        for (const auto& [callback, scale] : axis_bindings[event.get_key()]) {
            callback(scale);
        }
    });

    event_dispatcher->register_global_handler<KeyReleasedEvent>([](const KeyReleasedEvent& event) {
        for (const auto& callback : bindings_released[event.get_key()]) {
            callback();
        }

        for (const auto& callback : axis_bindings[event.get_key()] | std::views::keys) {
            callback(0.0f);
        }
    });

    event_dispatcher->register_global_handler<MouseButtonPressedEvent>([](const MouseButtonPressedEvent& event) {
        for (const auto& callback : bindings_pressed[event.get_button()]) {
            callback();
        }
    });

    event_dispatcher->register_global_handler<MouseButtonReleasedEvent>([](const MouseButtonReleasedEvent& event) {
        for (const auto& callback : bindings_released[event.get_button()]) {
            callback();
        }
    });

    static float last_x = 0.0;
    static float last_y = 0.0;
    event_dispatcher->register_global_handler<MouseMovedEvent>([](const MouseMovedEvent& event) {
        const float& x = event.get_x();
        const float& y = event.get_y();

        const float offset_x = x - last_x;
        const float offset_y = last_y - y; // reversed since y-coordinates go from bottom to top

        for (const auto& [callback, scale] : axis_bindings[InputCode::MOUSE_X]) {
            callback(offset_x * scale);
        }

        for (const auto& [callback, scale] : axis_bindings[InputCode::MOUSE_Y]) {
            callback(offset_y * scale);
        }

        last_x = x;
        last_y = y;
    });
}

#if defined(DE_PLATFORM_WINDOWS) || defined(DE_PLATFORM_MACOS)
int get_glfw_mouse_button_code(const InputCode input_code) {
    switch (input_code) { // NOLINT(clang-diagnostic-switch-enum)
            // @formatter:off
            // clang-format off
        case InputCode::MOUSE_BUTTON_1: return GLFW_MOUSE_BUTTON_1;
        case InputCode::MOUSE_BUTTON_2: return GLFW_MOUSE_BUTTON_2;
        case InputCode::MOUSE_BUTTON_3: return GLFW_MOUSE_BUTTON_3;
        case InputCode::MOUSE_BUTTON_4: return GLFW_MOUSE_BUTTON_4;
        case InputCode::MOUSE_BUTTON_5: return GLFW_MOUSE_BUTTON_5;
        case InputCode::MOUSE_BUTTON_6: return GLFW_MOUSE_BUTTON_6;
        case InputCode::MOUSE_BUTTON_7: return GLFW_MOUSE_BUTTON_7;
        case InputCode::MOUSE_BUTTON_8: return GLFW_MOUSE_BUTTON_8;
        default: return GLFW_KEY_UNKNOWN;
            // clang-format on
            // @formatter:on
    }
}

bool Input::is_action_pressed(const std::string& action) {
    const InputCode input_code = actions_mapping[action];
    // mouse button
    if (input_code >= InputCode::MOUSE_BUTTON_RANGE_START && input_code <= InputCode::MOUSE_BUTTON_RANGE_END) {
        const int status = glfwGetMouseButton(window->getNativeWindow(), get_glfw_mouse_button_code(input_code));
        return status == GLFW_PRESS;
    }
    // keyboard
    const int status = glfwGetKey(window->getNativeWindow(), static_cast<int>(input_code));
    return status == GLFW_PRESS || status == GLFW_REPEAT;
}
#else
    #error "Input::is_action_pressed not implemented for this platform"
#endif
