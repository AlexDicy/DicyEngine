#pragma once
#include "scene/entities/EntityScript.h"

class LightScript final : public EntityScript {
public:
    explicit LightScript(const Ref<Application>& app, const Ref<Entity>& entity, const Ref<DirectionalLight>& light, const Ref<Entity>& meshEntity);

    void onUpdate(float deltaTime) override;

private:
    Ref<DirectionalLight> light;
    Ref<Entity> meshEntity;
};
