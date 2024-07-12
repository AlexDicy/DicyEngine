#pragma once
#include "layer.h"
#include "window.h"
#include "gui/gui.h"
#include "rendering/renderer.h"
#include "rendering/shader_registry.h"


class Application {
public:
    Application();
    ~Application();

    void run();
    void update_frame(const std::unique_ptr<Context>& ctx) const;
    void register_layers();

    const Ref<Window>& get_window() const {
        return this->window;
    }

    const Ref<EventDispatcher>& get_event_dispatcher() const {
        return this->event_dispatcher;
    }

    const Ref<Renderer>& get_renderer() const {
        return this->renderer;
    }

    const Ref<ShaderRegistry>& get_shader_registry() const {
        return this->shader_registry;
    }

private:
    Ref<Window> window;
    Ref<EventDispatcher> event_dispatcher;
    std::unique_ptr<GUI> gui;
    Ref<Renderer> renderer;
    Ref<ShaderRegistry> shader_registry;
    std::vector<Layer*> layers = {};
    bool running;
    bool is_minimized;
};
