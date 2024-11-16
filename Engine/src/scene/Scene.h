#pragma once
#include "entities/Entity.h"
#include "components/Mesh.h"
#include "components/PointLight.h"
#include "components/Script.h"
#include "entt/entt.hpp"

class Scene : public std::enable_shared_from_this<Scene> {
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

private:
    Ref<entt::registry> registry;
    // keep entities alive
    // todo: use a better approach
    std::list<Ref<Entity>> entities;
    Ref<Camera> camera;
};
