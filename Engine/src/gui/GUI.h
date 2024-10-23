#pragma once
#include "Window.h"


class GUI {
protected:
    Ref<Window> window;

public:
    GUI(const Ref<Window>& window) : window(window) {}
    virtual ~GUI() = default;

    virtual void update(const std::unique_ptr<Context> &ctx) = 0;
};
