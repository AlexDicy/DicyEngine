#pragma once
#include "Serializer.h"

class EntitySerializer : public Serializer<Entity> {
public:
    void serialize(Entity& entity, toml::table& out) override;

    static bool shouldSerialize(const Entity& entity);
};
