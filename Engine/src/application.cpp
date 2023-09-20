#include "pch/enginepch.h"
#include "common.h"
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
        event_dispatcher->register_handler<WindowCloseEvent>([this](const WindowCloseEvent &) { this->running = false; });
        while (this->running) {
            window->update();
        }
    }
private:
    bool running;
};
