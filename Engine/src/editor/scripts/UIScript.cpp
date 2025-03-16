#include "pch/enginepch.h"
#include "UIScript.h"

#include <cef_app.h>

#include "Application.h"
#include "cef/OSRCefHandler.h"
#include "scene/Scene.h"

UIScript::UIScript(const Ref<Application>& app, const Ref<Entity>& entity) : EntityScript(app, entity) {
    this->handler = CefRefPtr(new OSRCefHandler(app));
}

void UIScript::onSpawn() {
    UITexture& uiTexture = this->getComponent<UITexture>();
    this->handler->setTexture(uiTexture.texture);
    CefWindowInfo windowInfo;
    windowInfo.SetAsWindowless(nullptr);
    CefBrowserSettings browserSettings;
    CefBrowserHost::CreateBrowser(windowInfo, this->handler, this->url, browserSettings, nullptr, nullptr);
}

void UIScript::onUpdate(const float deltaTime) {
    DE_PROFILE_FUNCTION();
    CefDoMessageLoopWork();
}
