#include "pch/enginepch.h"
#include "SceneSerializer.h"

#include "EntitySerializer.h"
#include "scene/entities/Entity.h"

void SceneSerializer::serialize(Scene& scene, toml::table& out) {
    toml::array entitiesArray;
    EntitySerializer entitySerializer;
    for (const Ref<Entity>& entity : scene.entities) {
        if (!EntitySerializer::shouldSerialize(*entity)) {
            continue;
        }
        toml::table entityTable;
        entitySerializer.serialize(*entity, entityTable);

        auto [path, meshIndex] = scene.getEntityModelInfo(entity);
        entityTable.emplace("model", toml::table({{"path", path}, {"meshIndex", meshIndex}}));

        entitiesArray.push_back(entityTable);
    }
    out.emplace("entities", std::move(entitiesArray));
}
