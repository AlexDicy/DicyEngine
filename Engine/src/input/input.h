#pragma once
#include "window.h"
#include "events/dispatcher.h"
#include "inputcodes.h"


class Input {
    // actions to keycode mapping
    static std::unordered_map<std::string, InputCode> actions_mapping;
    static std::unordered_map<InputCode, std::vector<std::function<void()>>> bindings_pressed;
    static std::unordered_map<InputCode, std::vector<std::function<void()>>> bindings_released;
    // axis mapping (axis name, [(keycode, value)])
    static std::unordered_map<std::string, std::unordered_map<InputCode, double>> axis_mapping;
    static std::unordered_map<InputCode, std::vector<std::pair<std::function<void(double)>, double>>> axis_bindings;

    static std::shared_ptr<Window> window;

public:
    static void init(const std::shared_ptr<EventDispatcher>& event_dispatcher, const std::shared_ptr<Window>& window);

    static void bind_action_pressed(const std::string &action, const std::function<void()> &callback) {
        bindings_pressed[actions_mapping[action]].push_back(callback);
    }

    static void bind_action_released(const std::string &action, const std::function<void()> &callback) {
        bindings_released[actions_mapping[action]].push_back(callback);
    }

    static void bind_axis(const std::string &axis, const std::function<void(double)> &callback) {
        for (const auto &[keycode, scale] : axis_mapping[axis]) {
            axis_bindings[keycode].emplace_back(callback, scale);
        }
    }

    static bool is_action_pressed(const std::string &action);
};
