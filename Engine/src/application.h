#pragma once
#include "layer.h"
#include "window.h"
#include "events/dispatcher.h"

class Application {
    std::shared_ptr<Window> window;
    bool running;

public:
    DE_API Application();

    DE_API void run();
};

void register_layers(std::vector<Layer *> &vector, EventDispatcher *event_dispatcher, const std::shared_ptr<Window> &window);
