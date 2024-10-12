#include "pch/enginepch.h"
#include "imgui_gui.h"

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


    ImGui_ImplGlfw_InitForOpenGL(this->window->get_native_window(), true);
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

    this->io->DisplaySize = ImVec2(static_cast<float>(this->window->get_width()), static_cast<float>(this->window->get_height()));

    ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2({0, 0}));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
    ImGui::Begin("Viewport");
    ImGui::SetWindowSize(ImVec2(800, 480), ImGuiCond_FirstUseEver);

    ImVec2 viewport_size = ImGui::GetContentRegionAvail();
    const Ref<Framebuffer> framebuffer = ctx->renderer->get_framebuffer();
    const auto texture_id = reinterpret_cast<void*>(framebuffer->get_color_texture_id()); // NOLINT(performance-no-int-to-ptr)
    ImGui::Image(texture_id, {viewport_size.x, viewport_size.y}, {0, 1}, {1, 0});
    if (this->previous_viewport_size.x != viewport_size.x || this->previous_viewport_size.y != viewport_size.y) { // NOLINT(clang-diagnostic-float-equal)
        ctx->renderer->set_viewport(0, 0, static_cast<int>(viewport_size.x), static_cast<int>(viewport_size.y));
        this->previous_viewport_size = viewport_size;
    }
    ImGui::End(); // Viewport
    ImGui::PopStyleVar(2);

    static bool show_demo_window = false;
    if (show_demo_window) {
        ImGui::ShowDemoWindow(&show_demo_window);
    }

    ImGui::Begin("Info");

    if (ImGui::Button("Toggle VSync")) { // returns true when clicked
        this->window->set_vsync(!this->window->is_vsync());
    }
    ImGui::SameLine();
    ImGui::Checkbox("Demo Window", &show_demo_window);

    const glm::vec3& camera_position = ctx->renderer->get_camera()->get_position();
    ImGui::Text("Camera position: %.2f, %.2f, %.2f", camera_position.x, camera_position.y, camera_position.z);
    ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / io->Framerate, io->Framerate);

    const std::vector<DE::Profiling::ProfilerResult> timings = DE::Profiling::get_profiler().get_timings();
    for (const auto& [name, duration] : timings) {
        ImGui::Text("%.3f ms - %s", static_cast<float>(duration) / 1000.0f, name.c_str());
    }

    ImGui::End(); // Info

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    GLFWwindow* backup_current_context = glfwGetCurrentContext();
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    glfwMakeContextCurrent(backup_current_context);
}
