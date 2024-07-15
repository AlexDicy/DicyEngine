#pragma once
#include "context.h"
#include "gui.h"
#include "imgui.h"
#include "window.h"


class ImGuiGUI : public GUI {
public:
    ImGuiGUI(const Ref<Window>& window);

    void update(const std::unique_ptr<Context>& ctx) override;

private:
    ImGuiIO* io;
    ImVec2 previous_viewport_size;
};
