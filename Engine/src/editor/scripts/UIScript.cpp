#include "pch/enginepch.h"
#include "UIScript.h"

#include "cef_app.h"

#include "Application.h"
#include "scene/Scene.h"

UIScript::UIScript(const Ref<Application>& app, const Ref<Entity>& entity) : EntityScript(app, entity) {}

void UIScript::onUpdate(const float deltaTime) {
    DE_PROFILE_FUNCTION();
    CefDoMessageLoopWork();
}
