#pragma once
#include "UIScript.h"
#include "scene/entities/EntityScript.h"

class EditorScript final : public EntityScript {
public:
    explicit EditorScript(const Ref<Application>& app, const Ref<Entity>& entity, const Ref<UIScript>& uiScript);

    /// returns -1 if no entity is selected, otherwise the ID of the selected entity
    int getSelectedEntityId() const {
        return this->selectedEntityId;
    }

    void drawSelectedEntity(const std::unique_ptr<Context>& ctx, const Mesh& mesh, const glm::mat4& transformMat) const;
    void drawOverlays(const std::unique_ptr<Context>& ctx) const;

private:
    Ref<UIScript> uiScript;
    int selectedEntityId;

    Ref<Mesh> overlaysMesh;
    Ref<Shader> overlaysShader;
    Ref<Shader> jumpFloodingPrepareShader;
    Ref<Shader> jumpFloodingShader;
};
