#include "pch/enginepch.h"
#include "UIScript.h"

#include "Application.h"
#include "Window.h"
#include "scene/Scene.h"

UIScript::UIScript(const Ref<Application>& app, const Ref<Entity>& entity) : EntityScript(app, entity) {
    const Ref<Window>& window = app->getWindow();
    const Ref<Renderer>& renderer = app->getRenderer();
    const Ref<Scene>& scene = entity->getScene();
}

void UIScript::onUpdate(const float deltaTime) {
    DE_PROFILE_FUNCTION();
}

void UIScript::onSpawn() {
}

void UIScript::onDestroy() {
}

void UIScript::onAwake() {
}

void UIScript::onSleep() {
}
