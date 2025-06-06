#include "pch/enginepch.h"
#include "Application.h"

#include "input/Input.h"
#include "layers/EditorLayer.h"
#include "layers/PhysicsLayer.h"
#include "layers/SceneLayer.h"
#include "layers/ScriptsLayer.h"
#include "physics/jolt/JoltPhysics.h"
#include "rendering/Renderer.h"
#include "rendering/opengl/OpenGLRenderer.h"
#include "scripting/JavaBindings.h"

Application::Application(const int argc, char* argv[]) : argc(argc), argv(argv) {
    Logger::init();
    this->running = true;
    this->isMinimized = false;
}

Application::~Application() = default;

void Application::initialize() {
    this->eventDispatcher.reset(EventDispatcher::get());
    this->physics = std::make_shared<JoltPhysics>();
    this->renderer = std::make_shared<OpenGLRenderer>();

    this->entityScriptRegistry = std::make_shared<EntityScriptRegistry>();
    this->shaderRegistry = std::make_shared<ShaderRegistry>(this->renderer);

    this->window = Window::create("DicyEngine", 1920, 1080);
    auto scene = std::make_shared<Scene>();
    this->currentCtx = std::make_unique<Context>(this->shared_from_this(), scene);

    this->eventDispatcher->registerGlobalHandler<WindowCloseEvent>([this](const WindowCloseEvent&) {
        this->running = false;
    });
    this->eventDispatcher->registerGlobalHandler<WindowResizeEvent>([this](const WindowResizeEvent& event) {
        this->isMinimized = event.getWidth() == 0 || event.getHeight() == 0;
    });
    this->eventDispatcher->registerGlobalHandler<WindowFramebufferResizeEvent>([this](const WindowFramebufferResizeEvent& event) {
        this->renderer->setFramebufferDimensions(event.getWidth(), event.getHeight());
        this->updateFrame(this->currentCtx); // keep drawing when user holds to resizes the window
    });

    Input::init(this->eventDispatcher, this->window);
    this->physics->init();
    this->renderer->init(this->window->getFramebufferWidth(), this->window->getFramebufferHeight());
    JavaBindings::init();
    registerLayers(this->currentCtx);
}

void Application::run() const {
    while (this->running) {
        this->updateFrame(this->currentCtx);
    }

    for (const auto& layer : layers) {
        // TODO: should be called by the editor
        layer->stop(this->currentCtx);
    }
}

void Application::updateFrame(const std::unique_ptr<Context>& ctx) const {
    DE::Profiling::getProfiler().clearTimings();
    this->window->pollEvents();

    ctx->setDeltaTime(this->window->getLastFrameTime());

    if (!this->isMinimized) {
        ctx->renderer->beginFrame();
        for (const auto& layer : layers) {
            layer->update(ctx);
        }
        ctx->renderer->endFrame();
    }

    this->renderer->drawToMainFramebuffer();
    this->window->update();
}

void Application::registerLayers(const std::unique_ptr<Context>& ctx) {
    this->layers.push_back(new ScriptsLayer(ctx));
    this->layers.push_back(new PhysicsLayer(ctx));
    this->layers.push_back(new SceneLayer(ctx));
    this->layers.push_back(new EditorLayer(ctx));
}
