﻿#include "pch/enginepch.h"
#include "Scene.h"

Scene::Scene() {
    this->registry = std::make_shared<entt::registry>();
}

Ref<Entity> Scene::createEntity() {
    entt::entity entity = this->registry->create();
    return std::make_shared<Entity>(this->shared_from_this(), this->registry, entity);
}