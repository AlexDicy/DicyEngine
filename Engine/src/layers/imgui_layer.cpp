#include "pch/enginepch.h"
#include "imgui_layer.h"

#include "GLFW/glfw3.h"
#include "platforms/opengl/imgui_impl_opengl3.h"


ImGuiLayer::ImGuiLayer(const unsigned index, EventDispatcher *event_dispatcher, const std::shared_ptr<Window> &window) : Layer(index, event_dispatcher) {
    this->window = window;
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    this->io = &ImGui::GetIO();
    io->BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
    io->BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

    ImGui_ImplOpenGL3_Init("#version 460");
    this->time = static_cast<float>(glfwGetTime());

    event_dispatcher->register_layer_handler<MouseMovedEvent>(index, [&](const MouseMovedEvent &event) {
        this->on_mouse_moved(event);
    });
    event_dispatcher->register_layer_handler<MouseButtonPressedEvent>(index, [&](const MouseButtonPressedEvent &event) {
        this->on_mouse_button_pressed(event);
    });
    event_dispatcher->register_layer_handler<MouseButtonReleasedEvent>(index, [&](const MouseButtonReleasedEvent &event) {
        this->on_mouse_button_released(event);
    });
    event_dispatcher->register_layer_handler<MouseScrolledEvent>(index, [&](const MouseScrolledEvent &event) {
        this->on_mouse_scrolled(event);
    });
    event_dispatcher->register_layer_handler<KeyPressedEvent>(index, [&](const KeyPressedEvent &event) {
        this->on_key_pressed(event);
    });
    event_dispatcher->register_layer_handler<KeyReleasedEvent>(index, [&](const KeyReleasedEvent &event) {
        this->on_key_released(event);
    });
    event_dispatcher->register_layer_handler<CharTypedEvent>(index, [&](const CharTypedEvent &event) {
        this->on_char_typed(event);
    });
    event_dispatcher->register_layer_handler<WindowResizeEvent>(index, [&](const WindowResizeEvent &event) {
        this->on_window_resized(event);
    });
}


void ImGuiLayer::update() {
    ImGuiIO &io = ImGui::GetIO();
    io.DisplaySize = ImVec2(static_cast<float>(this->window->get_width()), static_cast<float>(this->window->get_height()));

    const auto time = static_cast<float>(glfwGetTime());
    io.DeltaTime = time - this->time;
    this->time = time;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();

    static bool show_demo_window = true;
    ImGui::ShowDemoWindow(&show_demo_window);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool ImGuiLayer::on_mouse_button_pressed(const MouseButtonPressedEvent &event) const {
    this->io->MouseDown[event.get_button()] = true;
    return false;
}

bool ImGuiLayer::on_mouse_button_released(const MouseButtonReleasedEvent &event) const {
    this->io->MouseDown[event.get_button()] = false;
    return false;
}

bool ImGuiLayer::on_mouse_moved(const MouseMovedEvent &event) const {
    this->io->MousePos = ImVec2(static_cast<float>(event.get_x()), static_cast<float>(event.get_y()));
    return false;
}

bool ImGuiLayer::on_mouse_scrolled(const MouseScrolledEvent &event) const {
    this->io->MouseWheelH += static_cast<float>(event.get_offset_x());
    this->io->MouseWheel += static_cast<float>(event.get_offset_y());
    return false;
}

bool ImGuiLayer::on_key_pressed(const KeyPressedEvent &event) const {
    this->io->KeyCtrl = this->io->KeysDown[GLFW_KEY_LEFT_CONTROL] || this->io->KeysDown[GLFW_KEY_RIGHT_CONTROL];
    this->io->KeyShift = this->io->KeysDown[GLFW_KEY_LEFT_SHIFT] || this->io->KeysDown[GLFW_KEY_RIGHT_SHIFT];
    this->io->KeyAlt = this->io->KeysDown[GLFW_KEY_LEFT_ALT] || this->io->KeysDown[GLFW_KEY_RIGHT_ALT];
    this->io->KeySuper = this->io->KeysDown[GLFW_KEY_LEFT_SUPER] || this->io->KeysDown[GLFW_KEY_RIGHT_SUPER];
    io->AddKeyEvent(translate_key(event.get_key(), event.get_scancode()), true);
    return false;
}

bool ImGuiLayer::on_key_released(const KeyReleasedEvent &event) const {
    this->io->KeyCtrl = this->io->KeysDown[GLFW_KEY_LEFT_CONTROL] || this->io->KeysDown[GLFW_KEY_RIGHT_CONTROL];
    this->io->KeyShift = this->io->KeysDown[GLFW_KEY_LEFT_SHIFT] || this->io->KeysDown[GLFW_KEY_RIGHT_SHIFT];
    this->io->KeyAlt = this->io->KeysDown[GLFW_KEY_LEFT_ALT] || this->io->KeysDown[GLFW_KEY_RIGHT_ALT];
    this->io->KeySuper = this->io->KeysDown[GLFW_KEY_LEFT_SUPER] || this->io->KeysDown[GLFW_KEY_RIGHT_SUPER];
    io->AddKeyEvent(translate_key(event.get_key(), event.get_scancode()), false);
    return false;
}

bool ImGuiLayer::on_char_typed(const CharTypedEvent &event) const {
    this->io->AddInputCharacter(static_cast<unsigned short>(event.get_char()));
    return false;
}

bool ImGuiLayer::on_window_resized(const WindowResizeEvent &event) const {
    this->io->DisplaySize = ImVec2(static_cast<float>(event.get_width()), static_cast<float>(event.get_height()));
    return false;
}

static ImGuiKey get_imgui_key(int key) {
    switch (key) {
            // @formatter:off
            // clang-format off
        case GLFW_KEY_TAB: return ImGuiKey_Tab;
        case GLFW_KEY_LEFT: return ImGuiKey_LeftArrow;
        case GLFW_KEY_RIGHT: return ImGuiKey_RightArrow;
        case GLFW_KEY_UP: return ImGuiKey_UpArrow;
        case GLFW_KEY_DOWN: return ImGuiKey_DownArrow;
        case GLFW_KEY_PAGE_UP: return ImGuiKey_PageUp;
        case GLFW_KEY_PAGE_DOWN: return ImGuiKey_PageDown;
        case GLFW_KEY_HOME: return ImGuiKey_Home;
        case GLFW_KEY_END: return ImGuiKey_End;
        case GLFW_KEY_INSERT: return ImGuiKey_Insert;
        case GLFW_KEY_DELETE: return ImGuiKey_Delete;
        case GLFW_KEY_BACKSPACE: return ImGuiKey_Backspace;
        case GLFW_KEY_SPACE: return ImGuiKey_Space;
        case GLFW_KEY_ENTER: return ImGuiKey_Enter;
        case GLFW_KEY_ESCAPE: return ImGuiKey_Escape;
        case GLFW_KEY_APOSTROPHE: return ImGuiKey_Apostrophe;
        case GLFW_KEY_COMMA: return ImGuiKey_Comma;
        case GLFW_KEY_MINUS: return ImGuiKey_Minus;
        case GLFW_KEY_PERIOD: return ImGuiKey_Period;
        case GLFW_KEY_SLASH: return ImGuiKey_Slash;
        case GLFW_KEY_SEMICOLON: return ImGuiKey_Semicolon;
        case GLFW_KEY_EQUAL: return ImGuiKey_Equal;
        case GLFW_KEY_LEFT_BRACKET: return ImGuiKey_LeftBracket;
        case GLFW_KEY_BACKSLASH: return ImGuiKey_Backslash;
        case GLFW_KEY_RIGHT_BRACKET: return ImGuiKey_RightBracket;
        case GLFW_KEY_GRAVE_ACCENT: return ImGuiKey_GraveAccent;
        case GLFW_KEY_CAPS_LOCK: return ImGuiKey_CapsLock;
        case GLFW_KEY_SCROLL_LOCK: return ImGuiKey_ScrollLock;
        case GLFW_KEY_NUM_LOCK: return ImGuiKey_NumLock;
        case GLFW_KEY_PRINT_SCREEN: return ImGuiKey_PrintScreen;
        case GLFW_KEY_PAUSE: return ImGuiKey_Pause;
        case GLFW_KEY_KP_0: return ImGuiKey_Keypad0;
        case GLFW_KEY_KP_1: return ImGuiKey_Keypad1;
        case GLFW_KEY_KP_2: return ImGuiKey_Keypad2;
        case GLFW_KEY_KP_3: return ImGuiKey_Keypad3;
        case GLFW_KEY_KP_4: return ImGuiKey_Keypad4;
        case GLFW_KEY_KP_5: return ImGuiKey_Keypad5;
        case GLFW_KEY_KP_6: return ImGuiKey_Keypad6;
        case GLFW_KEY_KP_7: return ImGuiKey_Keypad7;
        case GLFW_KEY_KP_8: return ImGuiKey_Keypad8;
        case GLFW_KEY_KP_9: return ImGuiKey_Keypad9;
        case GLFW_KEY_KP_DECIMAL: return ImGuiKey_KeypadDecimal;
        case GLFW_KEY_KP_DIVIDE: return ImGuiKey_KeypadDivide;
        case GLFW_KEY_KP_MULTIPLY: return ImGuiKey_KeypadMultiply;
        case GLFW_KEY_KP_SUBTRACT: return ImGuiKey_KeypadSubtract;
        case GLFW_KEY_KP_ADD: return ImGuiKey_KeypadAdd;
        case GLFW_KEY_KP_ENTER: return ImGuiKey_KeypadEnter;
        case GLFW_KEY_KP_EQUAL: return ImGuiKey_KeypadEqual;
        case GLFW_KEY_LEFT_SHIFT: return ImGuiKey_LeftShift;
        case GLFW_KEY_LEFT_CONTROL: return ImGuiKey_LeftCtrl;
        case GLFW_KEY_LEFT_ALT: return ImGuiKey_LeftAlt;
        case GLFW_KEY_LEFT_SUPER: return ImGuiKey_LeftSuper;
        case GLFW_KEY_RIGHT_SHIFT: return ImGuiKey_RightShift;
        case GLFW_KEY_RIGHT_CONTROL: return ImGuiKey_RightCtrl;
        case GLFW_KEY_RIGHT_ALT: return ImGuiKey_RightAlt;
        case GLFW_KEY_RIGHT_SUPER: return ImGuiKey_RightSuper;
        case GLFW_KEY_MENU: return ImGuiKey_Menu;
        case GLFW_KEY_0: return ImGuiKey_0;
        case GLFW_KEY_1: return ImGuiKey_1;
        case GLFW_KEY_2: return ImGuiKey_2;
        case GLFW_KEY_3: return ImGuiKey_3;
        case GLFW_KEY_4: return ImGuiKey_4;
        case GLFW_KEY_5: return ImGuiKey_5;
        case GLFW_KEY_6: return ImGuiKey_6;
        case GLFW_KEY_7: return ImGuiKey_7;
        case GLFW_KEY_8: return ImGuiKey_8;
        case GLFW_KEY_9: return ImGuiKey_9;
        case GLFW_KEY_A: return ImGuiKey_A;
        case GLFW_KEY_B: return ImGuiKey_B;
        case GLFW_KEY_C: return ImGuiKey_C;
        case GLFW_KEY_D: return ImGuiKey_D;
        case GLFW_KEY_E: return ImGuiKey_E;
        case GLFW_KEY_F: return ImGuiKey_F;
        case GLFW_KEY_G: return ImGuiKey_G;
        case GLFW_KEY_H: return ImGuiKey_H;
        case GLFW_KEY_I: return ImGuiKey_I;
        case GLFW_KEY_J: return ImGuiKey_J;
        case GLFW_KEY_K: return ImGuiKey_K;
        case GLFW_KEY_L: return ImGuiKey_L;
        case GLFW_KEY_M: return ImGuiKey_M;
        case GLFW_KEY_N: return ImGuiKey_N;
        case GLFW_KEY_O: return ImGuiKey_O;
        case GLFW_KEY_P: return ImGuiKey_P;
        case GLFW_KEY_Q: return ImGuiKey_Q;
        case GLFW_KEY_R: return ImGuiKey_R;
        case GLFW_KEY_S: return ImGuiKey_S;
        case GLFW_KEY_T: return ImGuiKey_T;
        case GLFW_KEY_U: return ImGuiKey_U;
        case GLFW_KEY_V: return ImGuiKey_V;
        case GLFW_KEY_W: return ImGuiKey_W;
        case GLFW_KEY_X: return ImGuiKey_X;
        case GLFW_KEY_Y: return ImGuiKey_Y;
        case GLFW_KEY_Z: return ImGuiKey_Z;
        case GLFW_KEY_F1: return ImGuiKey_F1;
        case GLFW_KEY_F2: return ImGuiKey_F2;
        case GLFW_KEY_F3: return ImGuiKey_F3;
        case GLFW_KEY_F4: return ImGuiKey_F4;
        case GLFW_KEY_F5: return ImGuiKey_F5;
        case GLFW_KEY_F6: return ImGuiKey_F6;
        case GLFW_KEY_F7: return ImGuiKey_F7;
        case GLFW_KEY_F8: return ImGuiKey_F8;
        case GLFW_KEY_F9: return ImGuiKey_F9;
        case GLFW_KEY_F10: return ImGuiKey_F10;
        case GLFW_KEY_F11: return ImGuiKey_F11;
        case GLFW_KEY_F12: return ImGuiKey_F12;
        default: return ImGuiKey_None;
            // clang-format on
            // @formatter:on
    }
}

static ImGuiKey translate_key(unsigned int key, int scancode) {
    const char *key_name = glfwGetKeyName(static_cast<int>(key), scancode);
    if (key_name && key_name[0] != 0 && key_name[1] == 0) {
        const char char_names[] = "`-=[]\\,;\'./";
        const int char_keys[] = {GLFW_KEY_GRAVE_ACCENT, GLFW_KEY_MINUS,     GLFW_KEY_EQUAL,      GLFW_KEY_LEFT_BRACKET, GLFW_KEY_RIGHT_BRACKET, GLFW_KEY_BACKSLASH,
                                 GLFW_KEY_COMMA,        GLFW_KEY_SEMICOLON, GLFW_KEY_APOSTROPHE, GLFW_KEY_PERIOD,       GLFW_KEY_SLASH,         0};
        IM_ASSERT(IM_ARRAYSIZE(char_names) == IM_ARRAYSIZE(char_keys));
        if (key_name[0] >= '0' && key_name[0] <= '9') {
            return get_imgui_key(GLFW_KEY_0 + (key_name[0] - '0'));
        }
        if (key_name[0] >= 'A' && key_name[0] <= 'Z') {
            return get_imgui_key(GLFW_KEY_A + (key_name[0] - 'A'));
        }
        if (key_name[0] >= 'a' && key_name[0] <= 'z') {
            return get_imgui_key(GLFW_KEY_A + (key_name[0] - 'a'));
        }
        if (const char *p = strchr(char_names, key_name[0])) {
            return get_imgui_key(char_keys[p - char_names]);
        }
    }
    return get_imgui_key(static_cast<int>(key));
}
