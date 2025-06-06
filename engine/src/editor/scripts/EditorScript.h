#pragma once
#include "UIScript.h"
#include "scene/entities/EntityScript.h"

class EditorScript final : public EntityScript {
public:
    explicit EditorScript(const Ref<Application>& app, const Ref<Scene>& scene, const Ref<Entity>& entity, const Ref<UIScript>& uiScript);

    /// returns -1 if no entity is selected, otherwise the ID of the selected entity
    int getSelectedEntityId() const {
        return this->selectedEntityId;
    }

    void drawSelectedEntity(const std::unique_ptr<Context>& ctx, const Mesh& mesh, const glm::mat4& transformMat) const;
    void drawOverlays(const std::unique_ptr<Context>& ctx) const;

private:
    std::vector<Ref<Entity>> addEntitiesForGizmo(const Ref<Renderer>& renderer, const std::string& path, glm::vec3 position, const Rotation& rotation = Rotation(),
                                                  glm::vec3 scale = glm::vec3(1.0f)) const;
    std::vector<Ref<Entity>> addEntitiesForGizmo(const Ref<Renderer>& renderer, const std::vector<Model>& models, glm::vec3 position, const Rotation& rotation = Rotation(),
                                                  glm::vec3 scale = glm::vec3(1.0f)) const;

    Ref<Scene> scene;
    Ref<UIScript> uiScript;
    int selectedEntityId;

    Ref<Mesh> overlaysMesh;
    Ref<Shader> overlaysShader;
    Ref<Shader> jumpFloodingPrepareShader;
    Ref<Shader> jumpFloodingShader;
};
