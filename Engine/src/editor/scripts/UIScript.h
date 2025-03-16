#pragma once
#include "scene/entities/EntityScriptJava.h"

class UIScript final : public EntityScript {
public:
    explicit UIScript(const Ref<Application>& app, const Ref<Entity>& entity);

    void onUpdate(float deltaTime) override;
    void onSpawn() override;
    void onDestroy() override;
    void onAwake() override;
    void onSleep() override;

private:
    Ref<EntityScriptJava> script;
};
