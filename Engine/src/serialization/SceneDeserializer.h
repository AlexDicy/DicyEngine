#pragma once
#include "Context.h"
#include "Deserializer.h"
#include "scene/Scene.h"

#include <toml++/toml.hpp>

class SceneDeserializer : public Deserializer<Scene> {
public:
    /**
     * Deserialize requires an already created scene to deserialize into
     * it will add all the entities to the existing scene
     */
    static void deserialize(const std::unique_ptr<Context>& ctx, Scene& scene, toml::table& in);
};
