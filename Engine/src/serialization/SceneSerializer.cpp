#include "pch/enginepch.h"
#include "SceneSerializer.h"

#include "EntitySerializer.h"

void SceneSerializer::serialize(Scene& scene, toml::table& out) {
    toml::array entitiesArray;
    EntitySerializer entitySerializer;
    for (const Ref<Entity>& entity : scene.entities) {
        toml::table entityTable;
        entitySerializer.serialize(*entity, entityTable);
        entitiesArray.push_back(entityTable);
    }
    out.emplace("entities", std::move(entitiesArray));
}
