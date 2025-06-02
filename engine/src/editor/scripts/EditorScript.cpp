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
        this->selectedEntityId = entityId;
        this->uiScript->updateSelectedEntity(entityId);
    });

    this->uiScript->registerCallback("selectEntity", [this](const Callback& callback) {
        const int entityId = callback.getInt(0);
        this->selectedEntityId = entityId;
        callback.success();
    });
}
