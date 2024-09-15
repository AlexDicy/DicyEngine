#pragma once
#include <entt/entt.hpp>

class Entity {
public:
    Entity(const Ref<entt::registry>& registry, const entt::entity& entity) : registry(registry), entity(entity) {}

    template <typename... T>
    [[nodiscard]] bool has() const {
        return this->registry->all_of<T...>(this->entity);
    }

    template <typename T>
    [[nodiscard]] T& get() const {
        return this->registry->get<T>(this->entity);
    }

    template <typename T, typename... Args>
    T& add(Args&&... args) {
        return this->registry->emplace<T>(this->entity, std::forward<Args>(args)...);
    }

private:
    Ref<entt::registry> registry;
    entt::entity entity;
};
