#include "pch/enginepch.h"
#include "EntitySerializer.h"

#include "TransformSerializer.h"
#include "scene/entities/Entity.h"

void EntitySerializer::serialize(Entity& entity, toml::table& out) {
    TransformSerializer transformSerializer;
    toml::table transformTable;
    transformSerializer.serialize(*entity.getTransform(), transformTable);
    out.emplace("transform", std::move(transformTable));
}

bool EntitySerializer::shouldSerialize(const Entity& entity) {
    return entity.persistent;
}
