#include "pch/enginepch.h"
#include "application.h"

#include "glad/gl.h"
#include "layers/imgui_layer.h"


Application::Application() {
    logger::init();
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
        glClearColor(0.1f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        for (const auto &layer : layers) {
            layer->update();
        }

        this->window->update();
    }
}


void register_layers(std::vector<Layer *> &vector, EventDispatcher *event_dispatcher, const std::shared_ptr<Window> &window) {
    int layer_index = 0;
    vector.push_back(new ImGuiLayer(layer_index++, event_dispatcher, window));
}
