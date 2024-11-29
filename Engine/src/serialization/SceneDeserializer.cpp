#include "pch/enginepch.h"
#include "SceneDeserializer.h"

#include "TransformDeserializer.h"
#include "scene/entities/Entity.h"
#include "scene/models/ModelImporter.h"

#include <stack>

void SceneDeserializer::deserialize(const Ref<Renderer>& renderer, Scene& scene, toml::table& in) {
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

            Ref<Entity> entity = scene.createEntity();
            // entityDeserializer.deserialize(*entity, entityTable);

            // TODO: this should be done in EntityDeserializer
            // model
            if (entityTable.contains("model")) {
                const toml::table& modelTable = *entityTable["model"].as_table();
                const std::string path = modelTable["path"].value<std::string>().value();
                const long meshIndex = modelTable.get("meshIndex")->as_integer()->get();
                if (models.contains(path)) {
                    setEntityModel(renderer, scene, entity, models[path][meshIndex]);
                } else {
                    const std::vector<Model> loadedModels = ModelImporter::importFromFile(renderer, path);
                    models[path] = loadedModels;
                    setEntityModel(renderer, scene, entity, loadedModels[meshIndex]);
                }
            }

            // transform
            if (entityTable.contains("transform")) {
                Transform transform = TransformDeserializer::deserialize(*entityTable.get_as<toml::table>("transform"));
                entity->setTransform(transform.getPosition(), transform.getRotation(), transform.getScale());
            }

            // TODO: script

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

void SceneDeserializer::setEntityModel(const Ref<Renderer>& renderer, Scene& scene, const Ref<Entity>& entity, const Model& model) {
    const VertexData* vertexData = model.vertices.data();
    auto vertexDataFloats = reinterpret_cast<const float*>(vertexData);
    entity->add<Mesh>(renderer, vertexDataFloats, model.vertices.size() * sizeof(VertexData), model.indexes.data(), model.indexes.size(), model.material,
                      model.transformationMatrix);
    scene.setEntityModel(entity, model);
}
