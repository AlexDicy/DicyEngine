#include "pch/enginepch.h"
#include "input.h"

#include "GLFW/glfw3.h"

// actions
std::unordered_map<std::string, unsigned int> Input::actions_mapping;
std::unordered_map<unsigned int, std::vector<std::function<void()>>> Input::bindings_pressed;
std::unordered_map<unsigned int, std::vector<std::function<void()>>> Input::bindings_released;
// axes
std::unordered_map<std::string, std::unordered_map<unsigned int, float>> axis_mapping;
std::unordered_map<unsigned int, std::vector<std::pair<std::function<void(float)>, float>>> axis_bindings;


void Input::init(EventDispatcher *event_dispatcher) {
    // TODO: do not hardcode and use InputIDs or something
    actions_mapping["move_forward"] = GLFW_KEY_W;
    actions_mapping["move_left"] = GLFW_KEY_A;
    actions_mapping["move_backward"] = GLFW_KEY_S;
    actions_mapping["move_right"] = GLFW_KEY_D;
    actions_mapping["jump"] = GLFW_KEY_SPACE;

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
