#pragma once
#include "window.h"
#include "events/dispatcher.h"
#include "inputcodes.h"


class Input {
public:
    static void init(const Ref<EventDispatcher>& eventDispatcher, const Ref<Window>& window);

    static void bindActionPressed(const std::string& action, const std::function<void()>& callback) {
        bindingsPressed[actionsMapping[action]].push_back(callback);
    }

    static void bindActionReleased(const std::string& action, const std::function<void()>& callback) {
        bindingsReleased[actionsMapping[action]].push_back(callback);
    }

    static void bindAxis(const std::string& axis, const std::function<void(float)>& callback) {
        for (const auto& [keycode, scale] : axisMapping[axis]) {
            axisBindings[keycode].emplace_back(callback, scale);
        }
    }

    static void setAction(const std::string& action, const InputCode inputCode) {
        actionsMapping[action] = inputCode;
    }

    static bool isActionPressed(const std::string& action);

private:
    // actions to keycode mapping
    static std::unordered_map<std::string, InputCode> actionsMapping;
    static std::unordered_map<InputCode, std::vector<std::function<void()>>> bindingsPressed;
    static std::unordered_map<InputCode, std::vector<std::function<void()>>> bindingsReleased;
    // axis mapping (axis name, [(keycode, value)])
    static std::unordered_map<std::string, std::unordered_map<InputCode, float>> axisMapping;
    static std::unordered_map<InputCode, std::vector<std::pair<std::function<void(float)>, float>>> axisBindings;

    static Ref<Window> window;
};
