#pragma once
#include "input/InputCodes.h"

class Event {
public:
    bool isHandled() const {
        return this->handled;
    }
    void setHandled(const bool handled) {
        this->handled = handled;
    }

private:
    bool handled = false;
};

class WindowResizeEvent : public Event {
    unsigned int width;
    unsigned int height;

public:
    WindowResizeEvent(const unsigned int width, const unsigned int height) : width(width), height(height) {}

    unsigned int getWidth() const {
        return this->width;
    }

    unsigned int getHeight() const {
        return this->height;
    }
};

class WindowFramebufferResizeEvent : public Event {
    int width;
    int height;

public:
    WindowFramebufferResizeEvent(const int width, const int height) : width(width), height(height) {}

    int getWidth() const {
        return this->width;
    }

    int getHeight() const {
        return this->height;
    }
};

class WindowCloseEvent : public Event {};

struct KeyEventModifiers {
    bool ctrl;
    bool shift;
    bool alt;
    bool super;
};

class KeyEvent : public Event {
    InputCode key;
    int hostKey;
    int scancode;
    KeyEventModifiers modifiers;

public:
    KeyEvent(const InputCode key, const int hostKey, const int scancode, const KeyEventModifiers& modifiers) :
        key(key), hostKey(hostKey), scancode(scancode), modifiers(modifiers) {}

    InputCode getKey() const {
        return this->key;
    }

    int getHostKey() const {
        return this->hostKey;
    }

    int getScancode() const {
        return this->scancode;
    }

    bool isCtrl() const {
        return this->modifiers.ctrl;
    }

    bool isShift() const {
        return this->modifiers.shift;
    }

    bool isAlt() const {
        return this->modifiers.alt;
    }

    bool isSuper() const {
        return this->modifiers.super;
    }
};

class KeyPressedEvent : public KeyEvent {
    unsigned int repeatCount;

public:
    KeyPressedEvent(const InputCode key, const int hostKey, const int scancode, const KeyEventModifiers& modifiers, const unsigned int repeatCount) :
        KeyEvent(key, hostKey, scancode, modifiers), repeatCount(repeatCount) {}

    unsigned int getRepeatCount() const {
        return this->repeatCount;
    }
};

class KeyReleasedEvent : public KeyEvent {
public:
    KeyReleasedEvent(const InputCode key, const int hostKey, const int scancode, const KeyEventModifiers& modifiers) : KeyEvent(key, hostKey, scancode, modifiers) {}
};

class CharTypedEvent : public Event {
    unsigned int character;

public:
    explicit CharTypedEvent(const unsigned int character) : character(character) {}

    unsigned int getChar() const {
        return this->character;
    }
};

class MouseEvent : public Event {
    float x;
    float y;

protected:
    MouseEvent(const float x, const float y) : x(x), y(y) {}

public:
    float getX() const {
        return this->x;
    }

    float getY() const {
        return this->y;
    }
};

class MouseButtonPressedEvent : public MouseEvent {
    InputCode button;

public:
    explicit MouseButtonPressedEvent(const float x, const float y, const InputCode button) : MouseEvent(x, y), button(button) {}

    InputCode getButton() const {
        return this->button;
    }
};

class MouseButtonReleasedEvent : public MouseEvent {
    InputCode button;

public:
    explicit MouseButtonReleasedEvent(const float x, const float y, const InputCode button) : MouseEvent(x, y), button(button) {}

    InputCode getButton() const {
        return this->button;
    }
};

class MouseScrolledEvent : public MouseEvent {
    double offsetX;
    double offsetY;

public:
    MouseScrolledEvent(const float x, const float y, const double offsetX, const double offsetY) : MouseEvent(x, y), offsetX(offsetX), offsetY(offsetY) {}

    double getOffsetX() const {
        return this->offsetX;
    }

    double getOffsetY() const {
        return this->offsetY;
    }
};

class MouseMovedEvent : public MouseEvent {

public:
    MouseMovedEvent(const float x, const float y) : MouseEvent(x, y) {}
};
