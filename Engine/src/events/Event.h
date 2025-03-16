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

class WindowCloseEvent : public Event {};

class KeyPressedEvent : public Event {
    InputCode key;
    int scancode;
    unsigned int repeatCount;

public:
    KeyPressedEvent(const InputCode key, const int scancode, const unsigned int repeatCount) : key(key), scancode(scancode), repeatCount(repeatCount) {}

    InputCode getKey() const {
        return this->key;
    }

    int getScancode() const {
        return this->scancode;
    }

    unsigned int getRepeatCount() const {
        return this->repeatCount;
    }
};

class KeyReleasedEvent : public Event {
    InputCode key;
    int scancode;

public:
    explicit KeyReleasedEvent(const InputCode key, const int scancode) : key(key), scancode(scancode) {}

    InputCode getKey() const {
        return this->key; // TODO: should return InputID or similar
    }

    int getScancode() const {
        return this->scancode;
    }
};

class CharTypedEvent : public Event {
    unsigned int c;

public:
    explicit CharTypedEvent(const unsigned int c) : c(c) {}

    unsigned int getChar() const {
        return this->c;
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
