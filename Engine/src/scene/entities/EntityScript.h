#pragma once
#include "Application.h"
#include "scene/entities/Entity.h"

class EntityScript {
public:
    explicit EntityScript(const Ref<Application>& app, const Ref<Entity>& entity);
    virtual ~EntityScript() = default;

    virtual void onUpdate(float deltaTime) {}

    virtual void onSpawn() {}
    virtual void onDestroy() {}

    virtual void onAwake() {}
    virtual void onSleep() {}

    template <typename... T>
    [[nodiscard]] bool hasComponent() const {
        return this->entity->has<T...>();
    }

    template <typename T>
    [[nodiscard]] T& getComponent() const {
        return this->entity->get<T>();
    }

    template <typename T, typename... Args>
    T& addComponent(Args&&... args) {
        return this->entity->add<T>(std::forward<Args>(args)...);
    }

private:
    Ref<Entity> entity;
};