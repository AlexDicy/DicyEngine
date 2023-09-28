#pragma once
#include "imgui.h"
#include "layer.h"
#include "window.h"


class ImGuiLayer : public Layer {
    std::shared_ptr<Window> window;
    ImGuiIO *io;

public:
    ImGuiLayer(unsigned int index, EventDispatcher *event_dispatcher, const std::shared_ptr<Window> &window);

    void update() override;
};
