#include "pch/enginepch.h"
#include "imgui_layer.h"


#include <GLFW/glfw3.h>

void ImGuiLayer::update() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();

    ImGuiIO &io = ImGui::GetIO();
    io.DisplaySize = ImVec2(static_cast<float>(this->window->get_width()), static_cast<float>(this->window->get_height()));

    const auto time = static_cast<float>(glfwGetTime());
    io.DeltaTime = time - this->time;
    this->time = time;
    
    static bool show_demo_window = true;
    ImGui::ShowDemoWindow(&show_demo_window);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
