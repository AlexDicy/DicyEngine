#include "pch/enginepch.h"
#include "application.h"

#include "layers/imgui_layer.h"


Application::Application() {
    this->running = true;
}

void Application::run() {
    this->window = Window::create("DicyEngine", 1280, 720);

    const auto event_dispatcher = EventDispatcher::get();
    std::vector<Layer *> layers = {};
    register_layers(layers, event_dispatcher, window);

    event_dispatcher->register_global_handler<WindowCloseEvent>([this](const WindowCloseEvent &) {
        this->running = false;
    });

    while (this->running) {
        for (const auto &layer : layers) {
            layer->update();
        }

        window->update();
    }
}


void register_layers(std::vector<Layer *> &vector, EventDispatcher *event_dispatcher, std::shared_ptr<Window> window) {
    int layer_index = 0;
    vector.push_back(new ImGuiLayer(layer_index++, event_dispatcher, window));
}
