#include "pch/enginepch.h"
#include "EntityDeserializer.h"

#include "TransformDeserializer.h"
#include "scene/models/ModelImporter.h"

void EntityDeserializer::deserialize(const Ref<Renderer>& renderer, Scene& scene, const Ref<Entity>& entity, const toml::table& in,
                                     std::unordered_map<std::string, std::vector<Model>>& modelsCache) {
    // hierarchy is handled by the scene deserializer

    // model
    if (in.contains("model")) {
        const toml::table& modelTable = *in["model"].as_table();
        const std::string path = modelTable["path"].value<std::string>().value();
        const long meshIndex = modelTable.get("meshIndex")->as_integer()->get();
        if (modelsCache.contains(path)) {
            setEntityModel(renderer, scene, entity, modelsCache[path][meshIndex]);
        } else {
            const std::vector<Model> loadedModels = ModelImporter::importFromFile(renderer, path);
            modelsCache[path] = loadedModels;
            setEntityModel(renderer, scene, entity, loadedModels[meshIndex]);
        }
    }

    // transform
    if (in.contains("transform")) {
        Transform transform = TransformDeserializer::deserialize(*in.get_as<toml::table>("transform"));
        entity->setTransform(transform.getPosition(), transform.getRotation(), transform.getScale());
    }

    // TODO: script
}

void EntityDeserializer::setEntityModel(const Ref<Renderer>& renderer, Scene& scene, const Ref<Entity>& entity, const Model& model) {
    const VertexData* vertexData = model.vertices.data();
    auto vertexDataFloats = reinterpret_cast<const float*>(vertexData);
    entity->add<Mesh>(renderer, vertexDataFloats, model.vertices.size() * sizeof(VertexData), model.indexes.data(), model.indexes.size(), model.material,
                      model.transformationMatrix);
    // scene.setEntityModel(entity, model);
}
