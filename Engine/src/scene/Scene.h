#pragma once
#include "entities/Entity.h"
#include "components/Mesh.h"
#include "components/PointLight.h"
#include "components/Script.h"
#include "entt/entt.hpp"
#include "models/Model.h"

class Scene : public std::enable_shared_from_this<Scene> {
    friend class SceneSerializer;

public:
    Scene();

    Ref<Entity> createEntity();

    auto getMeshes() const {
        return this->registry->view<Mesh, Transform>();
    }

    auto getPointLights() const {
        return this->registry->view<PointLight>();
    }

    auto getScripts() const {
        return this->registry->view<Script>();
    }

    const Ref<Camera>& getCamera() const {
        return this->camera;
    }

    void setCamera(const Ref<Camera>& camera) {
        this->camera = camera;
    }

    void setEntityModel(const Ref<Entity>& entity, const Model& model) {
        entityModels[entity] = {model.path, model.meshIndex};
    }

protected:
    std::pair<std::string, unsigned int> getEntityModelInfo(const Ref<Entity>& entity) {
        return entityModels[entity];
    }

    // keep entities alive
    std::list<Ref<Entity>> entities;
    std::unordered_map<Ref<Entity>, std::pair<std::string, unsigned int>> entityModels;

private:
    Ref<entt::registry> registry;
    Ref<Camera> camera;
};
