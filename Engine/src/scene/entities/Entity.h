#pragma once

#include <entt/entt.hpp>


class EntityScriptJavaImpl;
class Scene;

class Entity {
    friend EntityScriptJavaImpl;

public:
    Entity(const Ref<Scene>& scene, const Ref<entt::registry>& registry, const entt::entity& entity) : scene(scene), registry(registry), entity(entity) {
        this->transform = &this->add<Transform>(); // every entity has a transform by default
    }

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

    Transform* getTransform() const {
        return this->transform;
    }

    void setTransform(const glm::vec3& position, const Rotation& rotation = Rotation(), const glm::vec3& scale = glm::vec3(1.0f)) const {
        this->transform->setPosition(position);
        this->transform->setRotation(rotation);
        this->transform->setScale(scale);
    }

    const Ref<Scene>& getScene() const {
        return this->scene;
    }

protected:
    const Ref<entt::registry>& getRegistry() const {
        return this->registry;
    }

    entt::entity getEntityId() const {
        return this->entity;
    }

private:
    Ref<Scene> scene;
    Ref<entt::registry> registry;
    entt::entity entity;

    Ref<Entity> parent;
    std::vector<Ref<Entity>> children;

    Transform* transform;
};
