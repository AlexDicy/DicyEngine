#pragma once
#include "layer.h"
#include "window.h"
#include "gui/gui.h"
#include "rendering/renderer.h"


class Application {
public:
    Application();
    ~Application();

    void run();
    void register_layers();

    std::shared_ptr<Window> get_window() const {
        return this->window;
    }

    std::shared_ptr<GUI> get_gui() const {
        return this->gui;
    }

    std::shared_ptr<EventDispatcher> get_event_dispatcher() const {
        return this->event_dispatcher;
    }

    const std::unique_ptr<Renderer>& get_renderer() const {
        return this->renderer;
    }

private:
    std::shared_ptr<Window> window;
    std::shared_ptr<GUI> gui;
    std::shared_ptr<EventDispatcher> event_dispatcher;
    std::unique_ptr<Renderer> renderer;
    std::vector<Layer*> layers = {};
    bool running;
};
