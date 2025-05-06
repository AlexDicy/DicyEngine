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
    actionsMapping["middle_click"] = InputCode::MOUSE_BUTTON_MIDDLE;
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

int Input::getWindowsKeyCode(InputCode code) {
    switch (code) { // NOLINT(clang-diagnostic-switch-enum)
            // clang-format off
        case InputCode::KEY_BACKSPACE: return 0x08; // VK_BACK
        case InputCode::KEY_TAB: return 0x09; // VK_TAB
        case InputCode::KEY_ENTER: return 0x0D; // VK_RETURN
        case InputCode::KEY_MENU: return 0x12; // VK_MENU
        case InputCode::KEY_PAUSE: return 0x13; // VK_PAUSE
        case InputCode::KEY_CAPS_LOCK: return 0x14; // VK_CAPITAL
        case InputCode::KEY_ESCAPE: return 0x1B; // VK_ESCAPE
        case InputCode::KEY_SPACE: return 0x20; // VK_SPACE
        case InputCode::KEY_PAGE_UP: return 0x21; // VK_PRIOR
        case InputCode::KEY_PAGE_DOWN: return 0x22; // VK_NEXT
        case InputCode::KEY_END: return 0x23; // VK_END
        case InputCode::KEY_HOME: return 0x24; // VK_HOME
        case InputCode::KEY_LEFT: return 0x25; // VK_LEFT
        case InputCode::KEY_UP: return 0x26; // VK_UP
        case InputCode::KEY_RIGHT: return 0x27; // VK_RIGHT
        case InputCode::KEY_DOWN: return 0x28; // VK_DOWN
        case InputCode::KEY_PRINT_SCREEN: return 0x2C; // VK_SNAPSHOT
        case InputCode::KEY_INSERT: return 0x2D; // VK_INSERT
        case InputCode::KEY_DELETE: return 0x2E; // VK_DELETE
        case InputCode::KEY_0: return 0x30; // `0`
        case InputCode::KEY_1: return 0x31; // `1`
        case InputCode::KEY_2: return 0x32; // `2`
        case InputCode::KEY_3: return 0x33; // `3`
        case InputCode::KEY_4: return 0x34; // `4`
        case InputCode::KEY_5: return 0x35; // `5`
        case InputCode::KEY_6: return 0x36; // `6`
        case InputCode::KEY_7: return 0x37; // `7`
        case InputCode::KEY_8: return 0x38; // `8`
        case InputCode::KEY_9: return 0x39; // `9`
        case InputCode::KEY_A: return 0x41; // `A`
        case InputCode::KEY_B: return 0x42; // `B`
        case InputCode::KEY_C: return 0x43; // `C`
        case InputCode::KEY_D: return 0x44; // `D`
        case InputCode::KEY_E: return 0x45; // `E`
        case InputCode::KEY_F: return 0x46; // `F`
        case InputCode::KEY_G: return 0x47; // `G`
        case InputCode::KEY_H: return 0x48; // `H`
        case InputCode::KEY_I: return 0x49; // `I`
        case InputCode::KEY_J: return 0x4A; // `J`
        case InputCode::KEY_K: return 0x4B; // `K`
        case InputCode::KEY_L: return 0x4C; // `L`
        case InputCode::KEY_M: return 0x4D; // `M`
        case InputCode::KEY_N: return 0x4E; // `N`
        case InputCode::KEY_O: return 0x4F; // `O`
        case InputCode::KEY_P: return 0x50; // `P`
        case InputCode::KEY_Q: return 0x51; // `Q`
        case InputCode::KEY_R: return 0x52; // `R`
        case InputCode::KEY_S: return 0x53; // `S`
        case InputCode::KEY_T: return 0x54; // `T`
        case InputCode::KEY_U: return 0x55; // `U`
        case InputCode::KEY_V: return 0x56; // `V`
        case InputCode::KEY_W: return 0x57; // `W`
        case InputCode::KEY_X: return 0x58; // `X`
        case InputCode::KEY_Y: return 0x59; // `Y`
        case InputCode::KEY_Z: return 0x5A; // `Z`
        case InputCode::KEY_LEFT_SUPER: return 0x5B; // VK_LWIN
        case InputCode::KEY_RIGHT_SUPER: return 0x5C; // VK_RWIN
        case InputCode::KEY_KP_0: return 0x60; // VK_NUMPAD0
        case InputCode::KEY_KP_1: return 0x61; // VK_NUMPAD1
        case InputCode::KEY_KP_2: return 0x62; // VK_NUMPAD2
        case InputCode::KEY_KP_3: return 0x63; // VK_NUMPAD3
        case InputCode::KEY_KP_4: return 0x64; // VK_NUMPAD4
        case InputCode::KEY_KP_5: return 0x65; // VK_NUMPAD5
        case InputCode::KEY_KP_6: return 0x66; // VK_NUMPAD6
        case InputCode::KEY_KP_7: return 0x67; // VK_NUMPAD7
        case InputCode::KEY_KP_8: return 0x68; // VK_NUMPAD8
        case InputCode::KEY_KP_9: return 0x69; // VK_NUMPAD9
        case InputCode::KEY_KP_MULTIPLY: return 0x6A; // VK_MULTIPLY
        case InputCode::KEY_KP_ADD: return 0x6B; // VK_ADD
        case InputCode::KEY_KP_SUBTRACT: return 0x6D; // VK_SUBTRACT
        case InputCode::KEY_KP_DECIMAL: return 0x6E; // VK_DECIMAL
        case InputCode::KEY_KP_DIVIDE: return 0x6F; // VK_DIVIDE
        case InputCode::KEY_F1: return 0x70; // VK_F1
        case InputCode::KEY_F2: return 0x71; // VK_F2
        case InputCode::KEY_F3: return 0x72; // VK_F3
        case InputCode::KEY_F4: return 0x73; // VK_F4
        case InputCode::KEY_F5: return 0x74; // VK_F5
        case InputCode::KEY_F6: return 0x75; // VK_F6
        case InputCode::KEY_F7: return 0x76; // VK_F7
        case InputCode::KEY_F8: return 0x77; // VK_F8
        case InputCode::KEY_F9: return 0x78; // VK_F9
        case InputCode::KEY_F10: return 0x79; // VK_F10
        case InputCode::KEY_F11: return 0x7A; // VK_F11
        case InputCode::KEY_F12: return 0x7B; // VK_F12
        case InputCode::KEY_F13: return 0x7C; // VK_F13
        case InputCode::KEY_F14: return 0x7D; // VK_F14
        case InputCode::KEY_F15: return 0x7E; // VK_F15
        case InputCode::KEY_F16: return 0x7F; // VK_F16
        case InputCode::KEY_F17: return 0x80; // VK_F17
        case InputCode::KEY_F18: return 0x81; // VK_F18
        case InputCode::KEY_F19: return 0x82; // VK_F19
        case InputCode::KEY_F20: return 0x83; // VK_F20
        case InputCode::KEY_F21: return 0x84; // VK_F21
        case InputCode::KEY_F22: return 0x85; // VK_F22
        case InputCode::KEY_F23: return 0x86; // VK_F23
        case InputCode::KEY_F24: return 0x87; // VK_F24
        case InputCode::KEY_NUM_LOCK: return 0x90; // VK_NUMLOCK
        case InputCode::KEY_SCROLL_LOCK: return 0x91; // VK_SCROLL
        case InputCode::KEY_LEFT_SHIFT: return 0xA0; // VK_LSHIFT
        case InputCode::KEY_RIGHT_SHIFT: return 0xA1; // VK_RSHIFT
        case InputCode::KEY_LEFT_CONTROL: return 0xA2; // VK_LCONTROL
        case InputCode::KEY_RIGHT_CONTROL: return 0xA3; // VK_RCONTROL
        case InputCode::KEY_LEFT_ALT: return 0xA4; // VK_LMENU
        case InputCode::KEY_RIGHT_ALT: return 0xA5; // VK_RMENU
        case InputCode::KEY_SEMICOLON: return 0xBA; // VK_OEM_1
        case InputCode::KEY_EQUAL: return 0xBB; // VK_OEM_PLUS
        case InputCode::KEY_COMMA: return 0xBC; // VK_OEM_COMMA
        case InputCode::KEY_MINUS: return 0xBD; // VK_OEM_MINUS
        case InputCode::KEY_PERIOD: return 0xBE; // VK_OEM_PERIOD
        case InputCode::KEY_SLASH: return 0xBF; // VK_OEM_2
        case InputCode::KEY_GRAVE_ACCENT: return 0xC0; // VK_OEM_3
        case InputCode::KEY_LEFT_BRACKET: return 0xDB; // VK_OEM_4
        case InputCode::KEY_BACKSLASH: return 0xDC; // VK_OEM_5
        case InputCode::KEY_RIGHT_BRACKET: return 0xDD; // VK_OEM_6
        case InputCode::KEY_APOSTROPHE: return 0xDE; // VK_OEM_7
        default: return static_cast<int>(code);
            // clang-format on
    }
}
