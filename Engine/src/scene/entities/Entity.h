#pragma once
#include "scene/components/Transform.h"

#include <entt/entt.hpp>


class Scene;

class Entity : public std::enable_shared_from_this<Entity> {
    friend class EntityScriptJavaImpl;
    friend class EntitySerializer;
    friend class Scene;

public:
    Entity(const Ref<Scene>& scene, const Ref<entt::registry>& registry, const entt::entity& id, const std::string& name) : scene(scene), registry(registry), id(id), name(name) {
        this->transform = &this->add<Transform>(); // every entity has a transform by default
        this->transform->setOwner(this);
    }

    template <typename... T>
    [[nodiscard]] bool has() const {
        return this->registry->all_of<T...>(this->id);
    }

    template <typename T>
    [[nodiscard]] T& get() const {
        return this->registry->get<T>(this->id);
    }

    template <typename T, typename... Args>
    T& add(Args&&... args) {
        return this->registry->emplace<T>(this->id, std::forward<Args>(args)...);
    }

    Transform* getTransform() const {
        return this->transform;
    }

    void setTransform(const glm::vec3& position, const Rotation& rotation = Rotation(), const glm::vec3& scale = glm::vec3(1.0f)) const {
        this->transform->setPosition(position);
        this->transform->setRotation(rotation);
        this->transform->setScale(scale);
    }

    bool hasParent() const {
        return this->parent != nullptr;
    }

    Ref<Entity> getParent() const {
        return this->parent;
    }

    void setParent(const Ref<Entity>& newParent) {
        Ref<Entity> sharedThis = this->shared_from_this();
        // remove from old parent, if any
        if (this->parent) {
            this->parent->children.remove(sharedThis);
            this->parent->childrenById.erase(this->getEntityId());
        }
        this->parent = newParent;
        // add to new parent, if any
        if (newParent) {
            newParent->children.push_back(sharedThis);
            newParent->childrenById[this->getEntityId()] = sharedThis;
        }
    }

    std::list<Ref<Entity>>& getChildren() {
        return this->children;
    }

    const Ref<Scene>& getScene() const {
        return this->scene;
    }

    void setPersistent(const bool persistent) {
        this->persistent = persistent;
    }

    entt::entity getEntityId() const {
        return this->id;
    }

    const std::string& getName() const {
        return this->name;
    }

protected:
    const Ref<entt::registry>& getRegistry() const {
        return this->registry;
    }

    Ref<Scene> scene;
    Ref<entt::registry> registry;
    entt::entity id;
    std::string name;
    // if the entity should be serialized and saved
    bool persistent = false;

    Ref<Entity> parent = nullptr;
    std::list<Ref<Entity>> children;
    std::unordered_map<entt::entity, Ref<Entity>> childrenById;

    Transform* transform;
};
