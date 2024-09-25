#pragma once
#include "scene/entities/entity_script.h"

class LightScript final : public EntityScript {
public:
    explicit LightScript(const Application* app, const Ref<Entity>& entity, const Ref<DirectionalLight>& light);

    void on_update(float delta_time) override;

private:
    Ref<DirectionalLight> light;
};
