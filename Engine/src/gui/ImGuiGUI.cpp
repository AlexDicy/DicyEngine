#include "pch/enginepch.h"
#include "ImGuiGUI.h"

#include "GLFW/glfw3.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

ImGuiGUI::ImGuiGUI(const Ref<Window>& window) : GUI(window) {
    this->window = window;
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    this->io = &ImGui::GetIO();
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 4.0f;
    style.FrameRounding = 2.0f;
    style.GrabRounding = 2.0f;
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.11f, 0.15f, 0.17f, 0.8f);
    style.Colors[ImGuiCol_TitleBgActive].w = 0.75f;


    ImGui_ImplGlfw_InitForOpenGL(this->window->getNativeWindow(), true);
    #ifdef OPENGL_4_6
    ImGui_ImplOpenGL3_Init("#version 460");
    #else
    ImGui_ImplOpenGL3_Init("#version 410");
    #endif
}

void ImGuiGUI::update(const std::unique_ptr<Context>& ctx) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    this->io->DisplaySize = ImVec2(static_cast<float>(this->window->getWidth()), static_cast<float>(this->window->getHeight()));

    ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2({0, 0}));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
    ImGui::Begin("Viewport");
    ImGui::SetWindowSize(ImVec2(800, 480), ImGuiCond_FirstUseEver);

    ImVec2 viewportSize = ImGui::GetContentRegionAvail();
    const Ref<RenderFramebuffer> framebuffer = ctx->renderer->getFramebuffer();
    const auto textureId = reinterpret_cast<void*>(framebuffer->getFinalColorTextureId()); // NOLINT(performance-no-int-to-ptr)
    ImGui::Image(textureId, {viewportSize.x, viewportSize.y}, {0, 1}, {1, 0});
    if (this->previousViewportSize.x != viewportSize.x || this->previousViewportSize.y != viewportSize.y) { // NOLINT(clang-diagnostic-float-equal)
        ctx->renderer->setViewport(0, 0, static_cast<int>(viewportSize.x), static_cast<int>(viewportSize.y));
        this->previousViewportSize = viewportSize;
    }
    ImGui::End(); // Viewport
    ImGui::PopStyleVar(2);

    static bool showDemoWindow = false;
    if (showDemoWindow) {
        ImGui::ShowDemoWindow(&showDemoWindow);
    }

    ImGui::Begin("Info");

    if (ImGui::Button("Toggle VSync")) { // returns true when clicked
        this->window->setVSync(!this->window->isVSync());
    }
    ImGui::SameLine();
    ImGui::Checkbox("Demo Window", &showDemoWindow);

    const glm::vec3& cameraPosition = ctx->renderer->getCamera()->getPosition();
    ImGui::Text("Camera position: %.2f, %.2f, %.2f", cameraPosition.x, cameraPosition.y, cameraPosition.z);
    ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / io->Framerate, io->Framerate);

    const std::vector<DE::Profiling::ProfilerResult> timings = DE::Profiling::getProfiler().getTimings();
    for (const auto& [name, duration] : timings) {
        ImGui::Text("%.3f ms - %s", static_cast<float>(duration) / 1000.0f, name.c_str());
    }

    ImGui::End(); // Info

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    GLFWwindow* backupCurrentContext = glfwGetCurrentContext();
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    glfwMakeContextCurrent(backupCurrentContext);
}
