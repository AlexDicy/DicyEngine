#include "pch/enginepch.h"
#include "common.h"
#include "layer.h"
#include "window.h"
#include "events/dispatcher.h"
#include "events/event.h"

#include <GLFW/glfw3.h>

class DE_API Application {
public:
    Application() {
        logger::init();
        this->running = true;
    }

    void run() {
        const auto window = Window::create("DicyEngine", 1280, 720);

        const auto event_dispatcher = EventDispatcher::get();
        std::vector<Layer *> layers = {};
        register_layers(layers, event_dispatcher);

        event_dispatcher->register_global_handler<WindowCloseEvent>([this](const WindowCloseEvent &) { this->running = false; });

        while (this->running) {
            window->update();
        }
    }

private:
    bool running;

    static void register_layers(std::vector<Layer *> &vector, EventDispatcher *event_dispatcher) {
        
    }
};
