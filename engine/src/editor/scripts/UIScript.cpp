// ReSharper disable CppTemplateArgumentsCanBeDeduced fix build on macos
#include "pch/enginepch.h"
#include "UIScript.h"

#include <cef_app.h>
#ifdef DE_PLATFORM_MACOS
    #include <wrapper/cef_library_loader.h>
#endif

#include "Application.h"
#include "cef/OSRCefHandler.h"
#include "scene/Scene.h"


UIScript::UIScript(const Ref<Application>& app, const Ref<Entity>& entity) : EntityScript(app, entity) {
    this->app = app;
    this->handler = CefRefPtr<OSRCefHandler>(new OSRCefHandler(app));

    auto& [texture] = this->getComponent<UITexture>();
    this->handler->setTexture(texture);

#ifdef DE_PLATFORM_WINDOWS
    this->cefThread = std::thread([this] {
        this->runCef();
    });
#else
    this->runCef();
#endif

    this->app->getEventDispatcher()->registerGlobalHandler<WindowResizeEvent>([this](const WindowResizeEvent& event) {
        this->handler->sendWindowResizeEvent(event);
    });

    this->app->getEventDispatcher()->registerGlobalHandler<MouseMovedEvent>([this](const MouseMovedEvent& event) {
        this->handler->sendMouseMoveEvent(event);
    });

    this->app->getEventDispatcher()->registerGlobalHandler<MouseButtonPressedEvent>([this](const MouseButtonPressedEvent& event) {
        this->handler->sendMouseButtonPressedEvent(event);
    });

    this->app->getEventDispatcher()->registerGlobalHandler<MouseButtonReleasedEvent>([this](const MouseButtonReleasedEvent& event) {
        this->handler->sendMouseButtonReleasedEvent(event);
    });

    this->app->getEventDispatcher()->registerGlobalHandler<MouseScrolledEvent>([this](const MouseScrolledEvent& event) {
        this->handler->sendMouseScrolledEvent(event);
    });

    this->app->getEventDispatcher()->registerGlobalHandler<KeyPressedEvent>([this](const KeyPressedEvent& event) {
        this->handler->sendKeyPressedEvent(event);
    });

    this->app->getEventDispatcher()->registerGlobalHandler<KeyReleasedEvent>([this](const KeyReleasedEvent& event) {
        this->handler->sendKeyReleasedEvent(event);
    });

    this->app->getEventDispatcher()->registerGlobalHandler<CharTypedEvent>([this](const CharTypedEvent& event) {
        this->handler->sendCharTypedEvent(event);
    });

    this->handler->registerCallback("toggleVSync", [this](const Callback& callback) {
        const bool vsync = this->app->getWindow()->isVSync();
        this->app->getWindow()->setVSync(!vsync);
        callback.success(!vsync);
    });

    this->handler->registerCallback("viewportResize", [this](const Callback& callback) {
        const unsigned int framebufferHeight = this->app->getWindow()->getFramebufferHeight();
        const int width = callback.getInt(2);
        const int height = callback.getInt(3);
        const int x = callback.getInt(0);
        const int y = static_cast<int>(framebufferHeight - callback.getInt(1) - height);
        this->app->getRenderer()->setViewport(x, y, width, height);
        callback.success();
    });

    this->handler->registerCallback("getSceneTree", [this](const Callback& callback) {
        MessageList entityList;
        for (const auto& entity : this->entity->getScene()->getEntities()) {
            if (entity->hasParent()) {
                continue;
            }
            entityList.appendDictionary(createEntityDictionary(entity));
        }
        callback.success(entityList);
    });

    static bool hasFocus = false;
    Input::setAction("focusBrowser", InputCode::KEY_F);
    Input::bindActionPressed("focusBrowser", [this] {
        hasFocus = !hasFocus;
        if (this->handler->getHost() == nullptr) {
            return;
        }
        this->handler->getHost()->SetFocus(hasFocus);
    });
}

void UIScript::onUpdate(const float deltaTime) {
    DE_PROFILE_FUNCTION();
    this->handler->processMainThreadTasks();
#ifdef DE_PLATFORM_WINDOWS
    this->handler->updateTextureIfNeeded();
#endif
    this->handler->updateFrameInfo(deltaTime);
    this->handler->updateProfilingInfo();
#ifdef DE_PLATFORM_MACOS
    CefDoMessageLoopWork();
#endif
}


void UIScript::updateSelectedEntity(const int entityId) const {
    if (this->handler->getHost() == nullptr) {
        return;
    }
    const auto args = BrowserMessageHandler::createMessage("updateSelectedEntity");
    args->appendInt(entityId);
    this->handler->getHost()->GetBrowser()->GetMainFrame()->SendProcessMessage(PID_RENDERER, args->getMessage());
}


bool UIScript::initializeCef() const {
#ifdef DE_PLATFORM_MACOS
    const CefMainArgs mainArgs(this->app->getArgc(), this->app->getArgv());
#else
    const CefMainArgs mainArgs;
#endif
    CefSettings settings;
    settings.no_sandbox = true;
    settings.windowless_rendering_enabled = true;
    const CefRefPtr<OSRCefApp> osrApp(new OSRCefApp);
    if (!CefInitialize(mainArgs, settings, osrApp.get(), nullptr)) {
        DE_ERROR("Failed to initialize CEF, exit code: {}", CefGetExitCode());
        return false;
    }
    return true;
}

void UIScript::runCef() const {
#ifdef DE_PLATFORM_MACOS
    if (CefScopedLibraryLoader libraryLoader; !libraryLoader.LoadInMain()) {
        DE_ERROR("Failed to load CEF libraries");
        return;
    }
#endif

    if (!this->initializeCef()) {
        return;
    }

    CefWindowInfo windowInfo;
    windowInfo.SetAsWindowless(nullptr);
    const CefBrowserSettings browserSettings;
    CefBrowserHost::CreateBrowser(windowInfo, this->handler, this->url, browserSettings, nullptr, nullptr);

#ifdef DE_PLATFORM_WINDOWS // main message loop can run on a separate thread on Windows
    CefRunMessageLoop();
    CefShutdown();
#endif
}

MessageDictionary UIScript::createEntityDictionary(const Ref<Entity>& entity) {
    MessageDictionary dictionary;
    dictionary.setInt("id", static_cast<int>(entity->getEntityId()));
    dictionary.setString("type", "Entity");
    dictionary.setString("name", entity->getName());
    if (!entity->getChildren().empty()) {
        MessageArguments children;
        for (const auto& child : entity->getChildren()) {
            children.appendDictionary(createEntityDictionary(child));
        }
        dictionary.setList("children", children);
    }
    return dictionary;
}
