#pragma once
#include "events/dispatcher.h"

class Input {
    static std::unordered_map<std::string, unsigned int> actions_mapping;
    static std::unordered_map<unsigned int, std::vector<std::function<void()>>> bindings_pressed;
    static std::unordered_map<unsigned int, std::vector<std::function<void()>>> bindings_released;

public:
    static void init(EventDispatcher *event_dispatcher);

    static void on_action_pressed(const std::string &action, const std::function<void()> &callback) {
        bindings_pressed[actions_mapping[action]].push_back(callback);
    }

    static void on_action_released(const std::string &action, const std::function<void()> &callback) {
        bindings_released[actions_mapping[action]].push_back(callback);
    }
};
