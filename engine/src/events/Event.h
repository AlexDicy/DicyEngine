#pragma once

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
    unsigned int key;
    unsigned int repeat_count;

public:
    KeyPressedEvent(const unsigned int key, const unsigned int repeat_count) : key(key), repeat_count(repeat_count) {
    }

    unsigned int get_key() const {
        return this->key;
    }

    unsigned int get_repeat_count() const {
        return this->repeat_count;
    }
};

class KeyReleasedEvent : public Event {
    unsigned int key;

public:
    explicit KeyReleasedEvent(const unsigned int key) : key(key) {
    }

    unsigned int get_key() const {
        return this->key;
    }
};

class MouseButtonPressedEvent : public Event {
    unsigned int button;

public:
    explicit MouseButtonPressedEvent(const unsigned int button) : button(button) {
    }

    unsigned int get_button() const {
        return this->button;
    }
};

class MouseButtonReleasedEvent : public Event {
    unsigned int button;

public:
    explicit MouseButtonReleasedEvent(const unsigned int button) : button(button) {
    }

    unsigned int get_button() const {
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
    double x;
    double y;

public:
    MouseMovedEvent(const double x, const double y) : x(x), y(y) {
    }

    double get_x() const {
        return this->x;
    }

    double get_y() const {
        return this->y;
    }
};
