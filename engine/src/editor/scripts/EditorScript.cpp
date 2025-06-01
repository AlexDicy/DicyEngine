#include "pch/enginepch.h"
#include "EditorScript.h"

#include "Application.h"


EditorScript::EditorScript(const Ref<Application>& app, const Ref<Entity>& entity, const Ref<UIScript>& uiScript) : EntityScript(app, entity), uiScript(uiScript) {
    app->getEventDispatcher()->registerGlobalHandler<MouseButtonPressedEvent>([this, app](const MouseButtonPressedEvent& event) {
        if (event.getButton() != InputCode::MOUSE_BUTTON_LEFT) {
            return;
        }

        const auto& [x, y, width, height] = app->getRenderer()->getViewport();
        const int mouseX = static_cast<int>(event.getX());
        const int mouseY = static_cast<int>(event.getY());
        if (mouseX < x || mouseX >= x + width || mouseY < y || mouseY >= y + height) {
            return; // mouse is outside the viewport
        }

        const int entityId = app->getRenderer()->getFramebuffer()->getMousePickingValue(mouseX, mouseY);
        DE_INFO("Selected entity: {}", entityId);
        this->uiScript->updateSelectedEntity(entityId);
    });
}

void EditorScript::onUpdate(const float deltaTime) {
    DE_PROFILE_FUNCTION();
}
