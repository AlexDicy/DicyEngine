#pragma once
#include "window.h"


class GUI {
protected:
    Ref<Window> window;

public:
    GUI(const Ref<Window> &window) : window(window) {
    }
    virtual ~GUI() = default;

    virtual void update() const = 0;
};
