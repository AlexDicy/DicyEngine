#include "pch/enginepch.h"
#include "Scene.h"

Scene::Scene() {
    this->registry = std::make_shared<entt::registry>();
}

Ref<Entity> Scene::createEntity() {
    entt::entity id = this->registry->create();
    auto entity = std::make_shared<Entity>(this->shared_from_this(), this->registry, id);
    this->entities.push_back(entity);
    return entity;
}

void Scene::destroyEntity(const Ref<Entity>& entity) {
    this->registry->destroy(entity->getEntityId());
    this->entities.remove(entity);
    this->entityModels.erase(entity);
}
