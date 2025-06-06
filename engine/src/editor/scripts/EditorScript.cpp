#include "pch/enginepch.h"
#include "EditorScript.h"

#include "Application.h"
#include "Context.h"
#include "scene/models/ModelImporter.h"
#include "scene/models/Plane.h"

#include <numbers>


EditorScript::EditorScript(const Ref<Application>& app, const Ref<Scene>& scene, const Ref<Entity>& entity, const Ref<UIScript>& uiScript) :
    EntityScript(app, entity), scene(scene), uiScript(uiScript) {
    this->overlaysMesh = Plane::create(app->getRenderer(), {});
    this->overlaysShader = app->getShaderRegistry()->load("../assets/shaders/editor-overlays-shader");
    this->jumpFloodingPrepareShader = app->getShaderRegistry()->load("../assets/shaders/jump-flooding-prepare");
    this->jumpFloodingShader = app->getShaderRegistry()->load("../assets/shaders/jump-flooding");

    // x,y,z indicator
    std::vector<Model> xyzModels = ModelImporter::importFromFile(app->getRenderer(), "../assets/models/arrows.glb");
    for (Model& arrow : xyzModels) {
        arrow.material.ignoreLighting = true;
    }
    std::vector<Ref<Entity>> xyzEntities = this->addEntitiesForGizmo(app->getRenderer(), xyzModels, {0.0f, 0.0f, 0.0f});

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

std::vector<Ref<Entity>> EditorScript::addEntitiesForGizmo(const Ref<Renderer>& renderer, const std::string& path, const glm::vec3 position, const Rotation& rotation,
                                                            const glm::vec3 scale) const {
    const std::vector<Model> models = ModelImporter::importFromFile(renderer, path);
    return this->addEntitiesForGizmo(renderer, models, position, rotation, scale);
}

std::vector<Ref<Entity>> EditorScript::addEntitiesForGizmo(const Ref<Renderer>& renderer, const std::vector<Model>& models, const glm::vec3 position, const Rotation& rotation,
                                                            const glm::vec3 scale) const {
    std::vector<Ref<Entity>> entities;
    for (const Model& model : models) {
        const VertexData* vertexData = model.vertices.data();
        auto vertexDataFloats = reinterpret_cast<const float*>(vertexData);
        const Material& material = model.material;
        Ref<Entity> entity = this->scene->createEntity();
        this->scene->setEntityModel(entity, model);
        entity->add<Mesh>(renderer, vertexDataFloats, model.vertices.size() * sizeof(VertexData), model.indexes.data(), model.indexes.size(), material, model.transformationMatrix);
        entity->setTransform(position, rotation, scale);
        entities.push_back(entity);
    }
    return entities;
}
