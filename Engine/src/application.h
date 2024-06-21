#pragma once
#include "layer.h"
#include "window.h"
#include "events/dispatcher.h"
#include "gui/gui.h"
#include "rendering/buffer.h"

class Application {
    std::shared_ptr<Window> window;
    std::shared_ptr<GUI> gui;
    bool running;

    std::unique_ptr<VertexBuffer> vertex_buffer;
    std::unique_ptr<IndexBuffer> index_buffer;

public:
    DE_API Application();
    DE_API ~Application();

    DE_API void run();

    std::shared_ptr<Window> get_window() const {
        return this->window;
    }

    std::shared_ptr<GUI> get_gui() const {
        return this->gui;
    }
};

void register_layers(std::vector<Layer*>& vector, EventDispatcher* event_dispatcher, const std::shared_ptr<Window>& window);
