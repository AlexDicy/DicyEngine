#include "pch/enginepch.h"
#include "Input.h"


#include "GLFW/glfw3.h"
#include <ranges>

// actions
std::unordered_map<std::string, InputCode> Input::actionsMapping;
std::unordered_map<InputCode, std::vector<std::function<void()>>> Input::bindingsPressed;
std::unordered_map<InputCode, std::vector<std::function<void()>>> Input::bindingsReleased;
// axes
std::unordered_map<std::string, std::unordered_map<InputCode, float>> Input::axisMapping;
std::unordered_map<InputCode, std::vector<std::pair<std::function<void(float)>, float>>> Input::axisBindings;

Ref<Window> Input::window;


void Input::init(const Ref<EventDispatcher>& eventDispatcher, const Ref<Window>& window) {
    Input::window = window;
    // TODO: do not hardcode
    actionsMapping["left_click"] = InputCode::MOUSE_BUTTON_LEFT;
    actionsMapping["right_click"] = InputCode::MOUSE_BUTTON_RIGHT;
    actionsMapping["move_forward"] = InputCode::KEY_W;
    actionsMapping["move_left"] = InputCode::KEY_A;
    actionsMapping["move_backward"] = InputCode::KEY_S;
    actionsMapping["move_right"] = InputCode::KEY_D;
    actionsMapping["jump"] = InputCode::KEY_SPACE;
    axisMapping["look_x"] = {{InputCode::MOUSE_X, 1.0}};
    axisMapping["look_y"] = {{InputCode::MOUSE_Y, 1.0}};
    axisMapping["move_x"] = {{InputCode::KEY_D, 1.0}, {InputCode::KEY_A, -1.0}};
    axisMapping["move_y"] = {{InputCode::KEY_W, 1.0}, {InputCode::KEY_S, -1.0}};

    eventDispatcher->registerGlobalHandler<KeyPressedEvent>([](const KeyPressedEvent& event) {
        if (event.getRepeatCount() > 0) {
            return;
        }

        for (const auto& callback : bindingsPressed[event.getKey()]) {
            callback();
        }

        for (const auto& [callback, scale] : axisBindings[event.getKey()]) {
            callback(scale);
        }
    });

    eventDispatcher->registerGlobalHandler<KeyReleasedEvent>([](const KeyReleasedEvent& event) {
        for (const auto& callback : bindingsReleased[event.getKey()]) {
            callback();
        }

        for (const auto& callback : axisBindings[event.getKey()] | std::views::keys) {
            callback(0.0f);
        }
    });

    eventDispatcher->registerGlobalHandler<MouseButtonPressedEvent>([](const MouseButtonPressedEvent& event) {
        for (const auto& callback : bindingsPressed[event.getButton()]) {
            callback();
        }
    });

    eventDispatcher->registerGlobalHandler<MouseButtonReleasedEvent>([](const MouseButtonReleasedEvent& event) {
        for (const auto& callback : bindingsReleased[event.getButton()]) {
            callback();
        }
    });

    static float lastX = 0.0;
    static float lastY = 0.0;
    eventDispatcher->registerGlobalHandler<MouseMovedEvent>([](const MouseMovedEvent& event) {
        const float& x = event.getX();
        const float& y = event.getY();

        const float offsetX = x - lastX;
        const float offsetY = lastY - y; // reversed since y-coordinates go from bottom to top

        for (const auto& [callback, scale] : axisBindings[InputCode::MOUSE_X]) {
            callback(offsetX * scale);
        }

        for (const auto& [callback, scale] : axisBindings[InputCode::MOUSE_Y]) {
            callback(offsetY * scale);
        }

        lastX = x;
        lastY = y;
    });
}

#if defined(DE_PLATFORM_WINDOWS) || defined(DE_PLATFORM_MACOS)
int getGLFWMouseButtonCode(const InputCode inputCode) {
    switch (inputCode) { // NOLINT(clang-diagnostic-switch-enum)
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

bool Input::isActionPressed(const std::string& action) {
    const InputCode inputCode = actionsMapping[action];
    // mouse button
    if (inputCode >= InputCode::MOUSE_BUTTON_RANGE_START && inputCode <= InputCode::MOUSE_BUTTON_RANGE_END) {
        const int status = glfwGetMouseButton(window->getNativeWindow(), getGLFWMouseButtonCode(inputCode));
        return status == GLFW_PRESS;
    }
    // keyboard
    const int status = glfwGetKey(window->getNativeWindow(), static_cast<int>(inputCode));
    return status == GLFW_PRESS || status == GLFW_REPEAT;
}
#else
    #error "Input::isActionPressed not implemented for this platform"
#endif
