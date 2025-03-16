#pragma once

class UIScript final : public EntityScript {
public:
    explicit UIScript(const Ref<Application>& app, const Ref<Entity>& entity);

    void onUpdate(float deltaTime) override;
};
