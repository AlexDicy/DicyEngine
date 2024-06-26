#pragma once
#include "input/inputcodes.h"

class Event {
public:
    bool is_handled() const {
        return this->handled;
    }
    void set_handled(const bool handled) {
        this->handled = handled;
    }

private:
    bool handled = false;
};

class WindowResizeEvent : public Event {
    unsigned int width;
    unsigned int height;

public:
    WindowResizeEvent(const unsigned int width, const unsigned int height) : width(width), height(height) {
    }

    unsigned int get_width() const {
        return this->width;
    }

    unsigned int get_height() const {
        return this->height;
    }
};

class WindowCloseEvent : public Event {};

class KeyPressedEvent : public Event {
    InputCode key;
    int scancode;
    unsigned int repeat_count;

public:
    KeyPressedEvent(const InputCode key, const int scancode, const unsigned int repeat_count) : key(key), scancode(scancode), repeat_count(repeat_count) {
    }

    InputCode get_key() const {
        return this->key;
    }

    int get_scancode() const {
        return this->scancode;
    }

    unsigned int get_repeat_count() const {
        return this->repeat_count;
    }
};

class KeyReleasedEvent : public Event {
    InputCode key;
    int scancode;

public:
    explicit KeyReleasedEvent(const InputCode key, const int scancode) : key(key), scancode(scancode) {
    }

    InputCode get_key() const {
        return this->key; // TODO: should return InputID or similar
    }

    int get_scancode() const {
        return this->scancode;
    }
};

class CharTypedEvent : public Event {
    unsigned int c;

public:
    explicit CharTypedEvent(const unsigned int c) : c(c) {
    }

    unsigned int get_char() const {
        return this->c;
    }
};

class MouseButtonPressedEvent : public Event {
    InputCode button;

public:
    explicit MouseButtonPressedEvent(const InputCode button) : button(button) {
    }

    InputCode get_button() const {
        return this->button;
    }
};

class MouseButtonReleasedEvent : public Event {
    InputCode button;

public:
    explicit MouseButtonReleasedEvent(const InputCode button) : button(button) {
    }

    InputCode get_button() const {
        return this->button;
    }
};

class MouseScrolledEvent : public Event {
    double offset_x;
    double offset_y;

public:
    MouseScrolledEvent(const double offset_x, const double offset_y) : offset_x(offset_x), offset_y(offset_y) {
    }

    double get_offset_x() const {
        return this->offset_x;
    }

    double get_offset_y() const {
        return this->offset_y;
    }
};

class MouseMovedEvent : public Event {
    float x;
    float y;

public:
    MouseMovedEvent(const float x, const float y) : x(x), y(y) {
    }

    float get_x() const {
        return this->x;
    }

    float get_y() const {
        return this->y;
    }
};
