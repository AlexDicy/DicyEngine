#pragma once
#include "UIScript.h"
#include "scene/entities/EntityScript.h"

class EditorScript final : public EntityScript {
public:
    explicit EditorScript(const Ref<Application>& app, const Ref<Entity>& entity, const Ref<UIScript>& uiScript);

    /// -1 if no entity is selected, otherwise the ID of the selected entity
    int getSelectedEntityId() const {
        return this->selectedEntityId;
    }

private:
    Ref<UIScript> uiScript;
    int selectedEntityId;
};
