#pragma once
#include "Context.h"
#include "GUI.h"
#include "imgui.h"
#include "Window.h"


class ImGuiGUI : public GUI {
public:
    ImGuiGUI(const Ref<Window>& window);

    void update(const std::unique_ptr<Context>& ctx) override;

private:
    ImGuiIO* io;
    ImVec2 previousViewportSize;
};
