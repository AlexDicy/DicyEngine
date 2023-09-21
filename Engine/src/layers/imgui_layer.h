#pragma once
#include "imgui.h"
#include "layer.h"
#include "window.h"
#include "platforms/opengl/imgui_impl_opengl3.h"


#include <GLFW/glfw3.h>

class ImGuiLayer : public Layer {
    float time = 0.0f;
    std::shared_ptr<Window> window;
public:
    ImGuiLayer(const unsigned int index, EventDispatcher *event_dispatcher, const std::shared_ptr<Window> window) : Layer(index, event_dispatcher) {
        this->window = window;
        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        ImGuiIO &io = ImGui::GetIO();
        io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
        io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

        ImGui_ImplOpenGL3_Init("#version 460");
        this->time = static_cast<float>(glfwGetTime());
    }

    void update() override;
};
