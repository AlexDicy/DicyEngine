#pragma once
#include "gui.h"
#include "imgui.h"
#include "window.h"


class ImGuiGUI : public GUI {
    ImGuiIO *io;

public:
    ImGuiGUI(const std::shared_ptr<Window> &window);

    void update() const override;
};
