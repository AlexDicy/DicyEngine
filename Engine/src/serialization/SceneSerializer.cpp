#include "pch/enginepch.h"
#include "SceneSerializer.h"

#include "EntitySerializer.h"
#include "scene/entities/Entity.h"

void SceneSerializer::serialize(Scene& scene, toml::table& out) {
    toml::array entitiesArray;
    EntitySerializer entitySerializer;

    std::vector<Ref<Entity>> rootEntities;
    for (const Ref<Entity>& entity : scene.entities) {
        // fixme (or not): if the root entity is not persistent, children will not be serialized either
        if (!entity->hasParent() && EntitySerializer::shouldSerialize(*entity)) {
            rootEntities.push_back(entity);
        }
    }

    for (const Ref<Entity>& entity : rootEntities) {
        toml::table entityTable;
        this->serializeEntityWithChildren(scene, entity, entityTable);
        entitiesArray.push_back(entityTable);
    }
    out.emplace("entities", std::move(entitiesArray));
}


// todo: rewrite without recursion, even though the scene should probably not be that nested in the first place
void SceneSerializer::serializeEntityWithChildren(Scene& scene, const Ref<Entity>& entity, toml::table& out) {
    EntitySerializer entitySerializer;
    entitySerializer.serialize(*entity, out);

    if (entity->has<Mesh>()) {
        auto [path, meshIndex] = scene.getEntityModelInfo(entity);
        out.emplace("model", toml::table({{"path", path}, {"meshIndex", meshIndex}}));
    }

    toml::array childrenArray;
    for (const Ref<Entity>& child : entity->getChildren()) {
        if (!EntitySerializer::shouldSerialize(*child)) {
            continue;
        }
        toml::table childTable;
        this->serializeEntityWithChildren(scene, child, childTable);
        childrenArray.push_back(childTable);
    }
    out.emplace("children", std::move(childrenArray));
}
