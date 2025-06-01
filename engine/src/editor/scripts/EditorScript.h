#pragma once
#include "UIScript.h"
#include "scene/entities/EntityScript.h"

class EditorScript final : public EntityScript {
public:
    explicit EditorScript(const Ref<Application>& app, const Ref<Entity>& entity, const Ref<UIScript>& uiScript);

    void onUpdate(float deltaTime) override;

private:
    Ref<UIScript> uiScript;
};
