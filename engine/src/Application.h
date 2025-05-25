#pragma once
#include "Window.h"
#include "physics/Physics.h"
#include "rendering/Renderer.h"


class Context;
class Layer;

class Application : public std::enable_shared_from_this<Application> {
public:
    Application(int argc, char* argv[]);
    ~Application();

    void initialize();
    void run() const;
    void updateFrame(const std::unique_ptr<Context>& ctx) const;
    void registerLayers(const std::unique_ptr<Context>& ctx);

    int getArgc() const {
        return this->argc;
    }

    char** getArgv() const {
        return this->argv;
    }

    const Ref<Window>& getWindow() const {
        return this->window;
    }

    const Ref<EventDispatcher>& getEventDispatcher() const {
        return this->eventDispatcher;
    }

    const Ref<Physics>& getPhysics() const {
        return this->physics;
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
    int argc;
    char** argv;

    Ref<Window> window;
    Ref<EventDispatcher> eventDispatcher;
    Ref<Physics> physics;
    Ref<Renderer> renderer;

    Ref<EntityScriptRegistry> entityScriptRegistry;
    Ref<ShaderRegistry> shaderRegistry;

    std::vector<Layer*> layers = {};

    bool running;
    bool isMinimized;
    std::unique_ptr<Context> currentCtx;
};
