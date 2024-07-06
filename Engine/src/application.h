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
    void update_frame(const std::unique_ptr<Context>& ctx) const;
    void register_layers();

    const Ref<Window>& get_window() const {
        return this->window;
    }

    const Ref<EventDispatcher>& get_event_dispatcher() const {
        return this->event_dispatcher;
    }

    const std::unique_ptr<Renderer>& get_renderer() const {
        return this->renderer;
    }

private:
    Ref<Window> window;
    Ref<EventDispatcher> event_dispatcher;
    std::unique_ptr<GUI> gui;
    std::unique_ptr<Renderer> renderer;
    std::vector<Layer*> layers = {};
    bool running;
};
