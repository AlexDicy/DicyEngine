#pragma once
#include "application.h"
#include "scene/entities/entity.h"

class EntityScript {
public:
    explicit EntityScript(const Ref<Application>& app, const Ref<Entity>& entity);
    virtual ~EntityScript() = default;

    virtual void on_update(float delta_time) {}

    virtual void on_spawn() {}
    virtual void on_destroy() {}

    virtual void on_awake() {}
    virtual void on_sleep() {}

    template <typename... T>
    [[nodiscard]] bool has_component() const {
        return this->entity->has<T...>();
    }

    template <typename T>
    [[nodiscard]] T& get_component() const {
        return this->entity->get<T>();
    }

    template <typename T, typename... Args>
    T& add_component(Args&&... args) {
        return this->entity->add<T>(std::forward<Args>(args)...);
    }

private:
    Ref<Entity> entity;
};
