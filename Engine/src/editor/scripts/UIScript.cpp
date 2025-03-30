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
    this->app->getBrowserApp()->getFrameInfoHandler()->updateFrameInfo(deltaTime);
    CefDoMessageLoopWork();
}
