#include "pch/enginepch.h"
#include "EntitySerializer.h"

#include "TransformSerializer.h"
#include "scene/components/Script.h"
#include "scene/entities/Entity.h"

std::string scriptTypeToString(const EntityScript::ScriptType type) {
    switch (type) {
        case EntityScript::ScriptType::NATIVE:
            return "NATIVE";
        case EntityScript::ScriptType::JVM:
            return "JVM";
    }
    return "UNKNOWN";
}

void EntitySerializer::serialize(Entity& entity, toml::table& out) {
    out.emplace("name", entity.getName());

    TransformSerializer transformSerializer;
    toml::table transformTable;
    transformSerializer.serialize(*entity.getTransform(), transformTable);
    out.emplace("transform", std::move(transformTable));

    if (entity.has<Script>()) {
        const Script& script = entity.get<Script>();
        if (!script.getName().empty()) {
            toml::table scriptTable;
            scriptTable.emplace("name", script.getName());
            const Ref<EntityScript> entityScript = script.getEntityScript();
            scriptTable.emplace("type", scriptTypeToString(entityScript->getType()));
            out.emplace("script", std::move(scriptTable));
        }
    }
}

bool EntitySerializer::shouldSerialize(const Entity& entity) {
    return entity.persistent;
}
