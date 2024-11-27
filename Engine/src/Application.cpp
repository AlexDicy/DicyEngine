#include "pch/enginepch.h"
#include "Application.h"

#include "Context.h"
#include "gui/ImGuiGUI.h"
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

void Application::run() {
    this->eventDispatcher.reset(EventDispatcher::get());
    this->renderer = std::make_shared<OpenGLRenderer>();

    this->entityScriptRegistry = std::make_shared<EntityScriptRegistry>();
    this->shaderRegistry = std::make_shared<ShaderRegistry>(this->renderer);

    this->window = Window::create("DicyEngine", 1920, 1080);
    this->gui = std::make_unique<ImGuiGUI>(window);
    const auto ctx = std::make_unique<Context>(renderer);

    this->eventDispatcher->registerGlobalHandler<WindowCloseEvent>([this](const WindowCloseEvent&) {
        this->running = false;
    });
    this->eventDispatcher->registerGlobalHandler<WindowResizeEvent>([this, &ctx](const WindowResizeEvent& event) {
        if (event.getWidth() == 0 || event.getHeight() == 0) {
            this->isMinimized = true;
            return;
        }
        this->isMinimized = false;
        this->updateFrame(ctx); // keep drawing when user holds to resizes the window
    });

    Input::init(this->eventDispatcher, this->window);
    this->renderer->init(0, 0, this->window->getWidth(), this->window->getHeight());
    JavaBindings::init();
    registerLayers(this->shared_from_this());

    while (this->running) {
        this->updateFrame(ctx);
    }
}

void Application::updateFrame(const std::unique_ptr<Context>& ctx) const {
    DE::Profiling::getProfiler().clearTimings();
    this->renderer->clean();

    ctx->setDeltaTime(this->window->getLastFrameTime());

    if (!this->isMinimized) {
        for (const auto& layer : layers) {
            layer->update(ctx);
        }
    }

    this->gui->update(ctx);
    this->window->update();
}

void Application::registerLayers(const Ref<Application>& app) {
    this->layers.push_back(new SceneLayer(app));
}
