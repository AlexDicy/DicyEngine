#pragma once
#include "window.h"


class GUI {
protected:
    std::shared_ptr<Window> window;

public:
    GUI(const std::shared_ptr<Window> &window) : window(window) {
    }
    virtual ~GUI() = default;

    virtual void update() const = 0;
};
