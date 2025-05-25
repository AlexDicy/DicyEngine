#include "pch/enginepch.h"
#include "SceneDeserializer.h"

#include "EntityDeserializer.h"
#include "scene/entities/Entity.h"

#include <stack>

void SceneDeserializer::deserialize(const std::unique_ptr<Context>& ctx, Scene& scene, toml::table& in) {
    const toml::array* entities = in["entities"].as_array();
    std::stack<std::pair<Ref<Entity>, const toml::array*>> parentEntityStack;
    parentEntityStack.emplace(nullptr, entities);

    // cache models
    std::unordered_map<std::string, std::vector<Model>> models;

    while (!parentEntityStack.empty()) {
        auto [currentParent, currentEntities] = parentEntityStack.top();
        parentEntityStack.pop();

        for (const toml::node& node : *currentEntities) {
            const toml::table& entityTable = *node.as_table();

            Ref<Entity> entity;
            if (entityTable.contains("name")) {
                entity = scene.createEntity(entityTable["name"].value<std::string>().value());
            } else {
                entity = scene.createEntity();
            }
            EntityDeserializer::deserialize(ctx, scene, entity, entityTable, models);

            // hierarchy
            if (currentParent) {
                entity->setParent(currentParent);
            }

            if (entityTable.contains("children") && !entityTable["children"].as_array()->empty()) {
                const toml::array* children = entityTable["children"].as_array();
                parentEntityStack.emplace(entity, children);
            }
        }
    }
}
