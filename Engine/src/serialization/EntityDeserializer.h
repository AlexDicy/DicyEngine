#pragma once
#include "Deserializer.h"
#include "scene/models/Model.h"

#include <toml++/toml.hpp>

class EntityDeserializer : public Deserializer<Entity> {
public:
    /**
     * Deserialization of an entity requires an already created basic entity to add the necessary info and components to.
     * Entities hierarchy is handled by the scene deserializer.
     *
     * To prevent loading the same model multiple times, a cache is used to store the loaded models.
     * Provided by the scene deserializer.
     */
    static void deserialize(const Ref<Renderer>& renderer, Scene& scene, const Ref<Entity>& entity, const toml::table& in,
                            std::unordered_map<std::string, std::vector<Model>>& modelsCache);

private:
    static void setEntityModel(const Ref<Renderer>& renderer, Scene& scene, const Ref<Entity>& entity, const Model& model);
};
