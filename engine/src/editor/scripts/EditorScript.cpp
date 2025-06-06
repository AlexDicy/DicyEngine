#include "pch/enginepch.h"
#include "EditorScript.h"

#include "Application.h"
#include "Context.h"
#include "scene/models/Plane.h"

#include <numbers>


EditorScript::EditorScript(const Ref<Application>& app, const Ref<Entity>& entity, const Ref<UIScript>& uiScript) : EntityScript(app, entity), uiScript(uiScript) {
    this->overlaysMesh = Plane::create(app->getRenderer(), {});
    this->overlaysShader = app->getShaderRegistry()->load("../assets/shaders/editor-overlays-shader");
    this->jumpFloodingPrepareShader = app->getShaderRegistry()->load("../assets/shaders/jump-flooding-prepare");
    this->jumpFloodingShader = app->getShaderRegistry()->load("../assets/shaders/jump-flooding");

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

void EditorScript::drawSelectedEntity(const std::unique_ptr<Context>& ctx, const Mesh& mesh, const glm::mat4& transformMat) const {
    ctx->renderer->drawJumpFloodingPrepare(mesh.vertexArray, transformMat, this->jumpFloodingPrepareShader);
}

void EditorScript::drawOverlays(const std::unique_ptr<Context>& ctx) const {
    // draw outline for selected entity
    constexpr float outlineWidth = 6.0f;
    constexpr glm::vec4 outlineColor = {0.07f, 0.66f, 0.96f, 0.96f};
    const int maxIndex = static_cast<int>(glm::ceil(glm::log(outlineWidth) / std::numbers::ln2));
    for (int i = maxIndex - 1; i >= 0; i--) {
        ctx->renderer->drawJumpFloodingPass(this->overlaysMesh->vertexArray, this->jumpFloodingShader, static_cast<int>(glm::pow(2, i)), false);
        ctx->renderer->drawJumpFloodingPass(this->overlaysMesh->vertexArray, this->jumpFloodingShader, static_cast<int>(glm::pow(2, i)), true);
    }
    ctx->renderer->drawEditorOverlays(this->overlaysMesh->vertexArray, this->overlaysShader, outlineColor, outlineWidth);
}
