#include "pch/enginepch.h"
#include "SceneDeserializer.h"

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
            const std::string name = entityTable["model"]["path"].value<std::string>().value_or("");
            DE_INFO("Deserializing entity with model: {0}, parent is null? {1}", name, currentParent == nullptr);

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
                const toml::table& transformTable = *entityTable.get_as<toml::table>("transform");
                toml::array positionArray = *transformTable.get_as<toml::array>("position");
                toml::array rotationArray = *transformTable.get_as<toml::array>("rotation");
                toml::array scaleArray = *transformTable.get_as<toml::array>("scale");
                glm::vec3 position = {
                    positionArray[0].value<double>().value(),
                    positionArray[1].value<double>().value(),
                    positionArray[2].value<double>().value(),
                };
                glm::vec3 rotation = {
                    rotationArray[0].value<double>().value(),
                    rotationArray[1].value<double>().value(),
                    rotationArray[2].value<double>().value(),
                };
                glm::vec3 scale = {
                    scaleArray[0].value<double>().value(),
                    scaleArray[1].value<double>().value(),
                    scaleArray[2].value<double>().value(),
                };
                entity->setTransform(position, Rotation(rotation), scale);
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
