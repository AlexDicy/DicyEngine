#pragma once
#include "Serializer.h"
#include "scene/Scene.h"

class SceneSerializer : public Serializer<Scene> {
public:
    void serialize(Scene& scene, toml::table& out) override;

private:
    void serializeEntityWithChildren(Scene& scene, const Ref<Entity>& entity, toml::table& out);
};
