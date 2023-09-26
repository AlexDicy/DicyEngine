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

std::shared_ptr<Window> Input::window;


void Input::init(EventDispatcher *event_dispatcher, const std::shared_ptr<Window> &window) {
    Input::window = window;
    // TODO: do not hardcode and use InputIDs or something
    actions_mapping["move_forward"] = KEY_W;
    actions_mapping["move_left"] = KEY_A;
    actions_mapping["move_backward"] = KEY_S;
    actions_mapping["move_right"] = KEY_D;
    actions_mapping["jump"] = KEY_SPACE;

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

    event_dispatcher->register_global_handler<MouseMovedEvent>([](const MouseMovedEvent &event) {
        // should use an input id for the mouse
    });
}

#ifdef DE_PLATFORM_WINDOWS
bool Input::is_action_pressed(const std::string &action) {
    const auto status = glfwGetKey(window->get_native_window(), actions_mapping[action]);  // NOLINT(bugprone-narrowing-conversions, cppcoreguidelines-narrowing-conversions)
    return status == GLFW_PRESS || status == GLFW_REPEAT;
}
#else
    #error "Input::is_action_pressed not implemented for this platform"
#endif
