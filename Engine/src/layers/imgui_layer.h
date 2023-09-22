#pragma once
#include "imgui.h"
#include "layer.h"
#include "window.h"


class ImGuiLayer : public Layer {
    float time = 0.0f;
    std::shared_ptr<Window> window;
    ImGuiIO *io;

public:
    ImGuiLayer(unsigned int index, EventDispatcher *event_dispatcher, std::shared_ptr<Window> window);

    void update() override;

private:
    bool on_mouse_button_pressed(const MouseButtonPressedEvent &event) const;
    bool on_mouse_button_released(const MouseButtonReleasedEvent &event) const;
    bool on_mouse_moved(const MouseMovedEvent &event) const;
    bool on_mouse_scrolled(const MouseScrolledEvent &event) const;
    bool on_key_pressed(const KeyPressedEvent &event) const;
    bool on_key_released(const KeyReleasedEvent &event) const;
    bool on_char_typed(const CharTypedEvent &event) const;
    bool on_window_resized(const WindowResizeEvent &event) const;
};


static ImGuiKey translate_key(unsigned int key, int scancode);
