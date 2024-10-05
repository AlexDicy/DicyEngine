#include "pch/enginepch.h"
#include "application.h"

#include "context.h"
#include "gui/imgui_gui.h"
#include "input/input.h"
#include "layers/scene_layer.h"
#include "rendering/renderer.h"
#include "platforms/opengl/opengl_renderer.h"

Application::Application() {
    Logger::init();
    this->running = true;
    this->is_minimized = false;
}

Application::~Application() = default;

void Application::run() {
    this->event_dispatcher.reset(EventDispatcher::get());
    this->renderer = std::make_shared<OpenGLRenderer>();
    this->shader_registry = std::make_shared<ShaderRegistry>(this->renderer);
    this->window = Window::create("DicyEngine", 1920, 1080);
    this->gui = std::make_unique<ImGuiGUI>(window);
    const auto ctx = std::make_unique<Context>(renderer);

    this->event_dispatcher->register_global_handler<WindowCloseEvent>([this](const WindowCloseEvent&) {
        this->running = false;
    });
    this->event_dispatcher->register_global_handler<WindowResizeEvent>([this, &ctx](const WindowResizeEvent& event) {
        if (event.get_width() == 0 || event.get_height() == 0) {
            this->is_minimized = true;
            return;
        }
        this->is_minimized = false;
        this->update_frame(ctx); // keep drawing when user holds to resizes the window
    });

    Input::init(this->event_dispatcher, this->window);
    this->renderer->init(0, 0, this->window->get_width(), this->window->get_height());
    register_layers(this->shared_from_this());

    while (this->running) {
        this->update_frame(ctx);
    }
}

void Application::update_frame(const std::unique_ptr<Context>& ctx) const {
    DE::Profiling::get_profiler().clear_timings();
    this->renderer->clean();

    ctx->set_delta_time(this->window->get_last_frame_time());

    if (!this->is_minimized) {
        for (const auto& layer : layers) {
            layer->update(ctx);
        }
    }

    this->gui->update(ctx);
    this->window->update();
}

void Application::register_layers(const Ref<Application>& app) {
    this->layers.push_back(new SceneLayer(app));
}
