#pragma once
#include "common.h"
#include "window.h"
#include "events/dispatcher.h"
#include "gui/gui.h"


class Layer;
class Renderer;

class Application {
public:
    DE_API Application();
    DE_API ~Application();

    DE_API void run();
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

    std::shared_ptr<Renderer> get_renderer() const {
        return this->renderer;
    }

private:
    std::shared_ptr<Window> window;
    std::shared_ptr<GUI> gui;
    std::shared_ptr<EventDispatcher> event_dispatcher;
    std::shared_ptr<Renderer> renderer;
    std::vector<Layer*> layers = {};
    bool running;
};
