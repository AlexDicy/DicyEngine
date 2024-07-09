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
}

Application::~Application() = default;

void Application::run() {
    this->event_dispatcher.reset(EventDispatcher::get());
    this->renderer.reset(new OpenGLRenderer());
    this->window = Window::create("DicyEngine", 1920, 1080);
    this->gui = std::make_unique<ImGuiGUI>(window);
    const auto ctx = std::make_unique<Context>(renderer);

    this->event_dispatcher->register_global_handler<WindowCloseEvent>([this](const WindowCloseEvent&) {
        this->running = false;
    });
    this->event_dispatcher->register_global_handler<WindowResizeEvent>([this, &ctx](const WindowResizeEvent& event) {
        if (event.get_width() == 0 || event.get_height() == 0) {
            return;
        }
        this->renderer->set_viewport(0, 0, event.get_width(), event.get_height());
        this->update_frame(ctx); // keep drawing when user holds to resizes the window
    });

    Input::init(this->event_dispatcher, window);
    this->renderer->init();
    register_layers();

    while (this->running) {
        this->update_frame(ctx);
    }
}

void Application::update_frame(const std::unique_ptr<Context>& ctx) const {
    this->renderer->clean();

    ctx->set_delta_time(this->window->get_last_frame_time());
    for (const auto& layer : layers) {
        layer->update(ctx);
    }

    this->gui->update();
    this->window->update();
}

void Application::register_layers() {
    this->layers.push_back(new SceneLayer(this));
}
