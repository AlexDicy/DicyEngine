#include "pch/enginepch.h"
#include "common.h"
#include "layer.h"
#include "window.h"
#include "events/dispatcher.h"
#include "events/event.h"
#include "layers\imgui_layer.h"

#include <GLFW/glfw3.h>

class DE_API Application {
    std::shared_ptr<Window> window = nullptr;
public:
    Application() {
        logger::init();
        this->running = true;
    }

    void run() {
        this->window = Window::create("DicyEngine", 1280, 720);

        const auto event_dispatcher = EventDispatcher::get();
        std::vector<Layer *> layers = {};
        register_layers(layers, event_dispatcher);

        event_dispatcher->register_global_handler<WindowCloseEvent>([this](const WindowCloseEvent &) {
            this->running = false;
        });

        while (this->running) {
            for (const auto &layer : layers) {
                layer->update();
            }

            this->window->update();
        }
    }

private:
    bool running;

    void register_layers(std::vector<Layer *> &vector, EventDispatcher *event_dispatcher) const {
        int layer_index = 0;
        vector.push_back(new ImGuiLayer(layer_index++, event_dispatcher, this->window));
    }
};
