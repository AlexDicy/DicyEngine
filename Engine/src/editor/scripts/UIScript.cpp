// ReSharper disable CppTemplateArgumentsCanBeDeduced fix build on macos
#include "pch/enginepch.h"
#include "UIScript.h"

#include <cef_app.h>

#include "Application.h"
#include "cef/OSRCefHandler.h"
#include "scene/Scene.h"

UIScript::UIScript(const Ref<Application>& app, const Ref<Entity>& entity) : EntityScript(app, entity) {
    this->app = app;
    this->handler = CefRefPtr<OSRCefHandler>(new OSRCefHandler(app));
}

void UIScript::onSpawn() {
    auto& [texture] = this->getComponent<UITexture>();
    this->handler->setTexture(texture);
    CefWindowInfo windowInfo;
    windowInfo.SetAsWindowless(nullptr);
    const CefBrowserSettings browserSettings;
    CefBrowserHost::CreateBrowser(windowInfo, this->handler, this->url, browserSettings, nullptr, nullptr);

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
    this->handler->updateFrameInfo(deltaTime);
    this->handler->updateProfilingInfo();
    CefDoMessageLoopWork();
}
