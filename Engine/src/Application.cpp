#include "pch/enginepch.h"
#include "Application.h"

#include "input/Input.h"
#include "layers/SceneLayer.h"
#include "rendering/Renderer.h"
#include "rendering/opengl/OpenGLRenderer.h"
#include "scripting/JavaBindings.h"

Application::Application() {
    Logger::init();
    this->running = true;
    this->isMinimized = false;
}

Application::~Application() = default;

void Application::initialize() {
    this->eventDispatcher.reset(EventDispatcher::get());
    this->renderer = std::make_shared<OpenGLRenderer>();

    this->entityScriptRegistry = std::make_shared<EntityScriptRegistry>();
    this->shaderRegistry = std::make_shared<ShaderRegistry>(this->renderer);

    this->window = Window::create("DicyEngine", 1920, 1080);
    this->currentCtx = std::make_unique<Context>(this->shared_from_this());

    this->eventDispatcher->registerGlobalHandler<WindowCloseEvent>([this](const WindowCloseEvent&) {
        this->running = false;
    });
    this->eventDispatcher->registerGlobalHandler<WindowResizeEvent>([this](const WindowResizeEvent& event) {
        if (event.getWidth() == 0 || event.getHeight() == 0) {
            this->isMinimized = true;
            return;
        }
        this->isMinimized = false;
        this->renderer->setWindowDimensions(event.getWidth(), event.getHeight());
        this->updateFrame(this->currentCtx); // keep drawing when user holds to resizes the window
    });

    Input::init(this->eventDispatcher, this->window);
    this->renderer->init(this->window->getWidth(), this->window->getHeight());
    JavaBindings::init();
    registerLayers(this->currentCtx);
}

void Application::run() const {
    while (this->running) {
        this->updateFrame(this->currentCtx);
    }
}

void Application::updateFrame(const std::unique_ptr<Context>& ctx) const {
    DE::Profiling::getProfiler().clearTimings();
    this->window->pollEvents();
    this->renderer->clear();

    ctx->setDeltaTime(this->window->getLastFrameTime());

    if (!this->isMinimized) {
        for (const auto& layer : layers) {
            layer->update(ctx);
        }
    }

    this->renderer->drawToMainFramebuffer();
    this->window->update();
}

void Application::registerLayers(const std::unique_ptr<Context>& ctx) {
    this->layers.push_back(new SceneLayer(ctx));
}
