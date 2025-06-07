#include "pch/enginepch.h"
#include "Scene.h"

Scene::Scene() {
    this->registry = std::make_shared<entt::registry>();
}

Ref<Entity> Scene::createEntity() {
    entt::entity id = this->registry->create();
    const std::string name = std::format("Entity ({})", static_cast<unsigned int>(id));
    return this->createEntityWithId(name, id);
}

Ref<Entity> Scene::createEntity(const std::string& name) {
    return this->createEntityWithId(name, this->registry->create());
}

Ref<Entity> Scene::createEntityWithId(const std::string& name, entt::entity id) {
    auto entity = std::make_shared<Entity>(this->shared_from_this(), this->registry, id, name);
    this->entities.push_back(entity);
    this->entitiesById[entt::to_integral(id)] = entity;
    return entity;
}

void Scene::destroyEntity(const Ref<Entity>& entity) {
    this->registry->destroy(entity->getEntityId());
    this->entities.remove(entity);
    this->entitiesById.erase(entt::to_integral(entity->getEntityId()));
    this->entityModels.erase(entity);
}

const Ref<Entity>& Scene::getEntity(const unsigned int id) const {
    return this->entitiesById.find(id)->second;
}
