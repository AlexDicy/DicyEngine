#pragma once
#include "Window.h"
#include "rendering/Renderer.h"


class GUI;
class Context;
class Layer;

class Application : public std::enable_shared_from_this<Application> {
public:
    Application();
    ~Application();

    void run();
    void updateFrame(const std::unique_ptr<Context>& ctx) const;
    void registerLayers(const std::unique_ptr<Context>& app);

    const Ref<Window>& getWindow() const {
        return this->window;
    }

    const Ref<EventDispatcher>& getEventDispatcher() const {
        return this->eventDispatcher;
    }

    const Ref<Renderer>& getRenderer() const {
        return this->renderer;
    }

    const Ref<EntityScriptRegistry>& getEntityScriptRegistry() const {
        return this->entityScriptRegistry;
    }

    const Ref<ShaderRegistry>& getShaderRegistry() const {
        return this->shaderRegistry;
    }

private:
    Ref<Window> window;
    Ref<EventDispatcher> eventDispatcher;
    std::unique_ptr<GUI> gui;
    Ref<Renderer> renderer;

    Ref<EntityScriptRegistry> entityScriptRegistry;
    Ref<ShaderRegistry> shaderRegistry;

    std::vector<Layer*> layers = {};

    bool running;
    bool isMinimized;
};
