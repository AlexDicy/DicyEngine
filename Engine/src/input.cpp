#include "pch/enginepch.h"
#include "input.h"

#include "GLFW/glfw3.h"

std::unordered_map<std::string, unsigned int> Input::actions_mapping;
std::unordered_map<unsigned int, std::vector<std::function<void()>>> Input::bindings_pressed;
std::unordered_map<unsigned int, std::vector<std::function<void()>>> Input::bindings_released;


void Input::init(EventDispatcher *event_dispatcher) {
    // TODO: do not hardcode
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
    });

    event_dispatcher->register_global_handler<KeyReleasedEvent>([](const KeyReleasedEvent &event) {
        for (const auto &callback : bindings_released[event.get_key()]) {
            callback();
        }
    });
}
