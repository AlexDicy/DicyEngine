#include "pch/enginepch.h"
#include "EditorLayer.h"

#include "scene/models/Plane.h"

EditorLayer::EditorLayer(const std::unique_ptr<Context>& ctx) : Layer(ctx) {
    Ref<Entity> uiEntity = this->scene->createEntity("CEF Editor UI");
    const auto uiMaterial = Material(ctx->renderer->createTexture(1, 1, 1, Texture::Format::BGRA, Texture::InternalFormat::RGBA8, std::array<unsigned char, 4>{0, 0, 0, 0}.data()));
    this->uiMesh = Plane::create(ctx->renderer, uiMaterial);
    uiEntity->add<UITexture>(uiMaterial.albedo);
    Script& uiScript = uiEntity->add<Script>(std::make_shared<UIScript>(ctx->app, uiEntity));
    this->uiShader = ctx->app->getShaderRegistry()->load("../assets/shaders/ui");

    Ref<Entity> editorEntity = this->scene->createEntity("Editor");
    this->editorScript = std::make_shared<EditorScript>(ctx->app, this->scene, editorEntity, std::static_pointer_cast<UIScript>(uiScript.getEntityScript()));
    editorEntity->add<Script>(editorScript);
}

void EditorLayer::update(const std::unique_ptr<Context>& ctx) {
    const int selectedEntity = this->editorScript->getSelectedEntityId();
    if (selectedEntity) {
        const auto meshesView = this->scene->getMeshes();
        for (const auto& entity : meshesView) {
            const unsigned int entityId = entt::to_integral(entity);
            const bool isSelected = selectedEntity >= 0 && std::cmp_equal(selectedEntity, entityId);
            if (!isSelected) {
                continue;
            }

            Transform& transform = meshesView.get<Transform>(entity);
            const Mesh& mesh = meshesView.get<Mesh>(entity);

            const glm::mat4 transformMat = transform.getAsMatrix() * mesh.transformationMatrix;

            this->editorScript->drawSelectedEntity(ctx, mesh, transformMat);
        }
        this->editorScript->drawOverlays(ctx);
    }

    ctx->renderer->drawUI(this->uiMesh->vertexArray, this->uiShader, this->uiMesh->material);
}
