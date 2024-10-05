#pragma once
#include "scene/entities/entity_script.h"

class LightScript final : public EntityScript {
public:
    explicit LightScript(const Ref<Application>& app, const Ref<Entity>& entity, const Ref<DirectionalLight>& light, const Ref<Entity>& mesh_entity);

    void on_update(float delta_time) override;

private:
    Ref<DirectionalLight> light;
    Ref<Entity> mesh_entity;
};
