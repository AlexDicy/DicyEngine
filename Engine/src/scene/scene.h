#pragma once
#include "entities/entity.h"
#include "components/mesh.h"
#include "components/point_light.h"
#include "components/script.h"
#include "components/transform.h"
#include "entt/entt.hpp"

class Scene : public std::enable_shared_from_this<Scene> {
public:
    Scene();

    Ref<Entity> create_entity();

    auto get_meshes() const {
        return this->registry->view<Mesh, Transform>();
    }

    auto get_point_lights() const {
        return this->registry->view<PointLight>();
    }

    auto get_scripts() const {
        return this->registry->view<Script>();
    }

    const Ref<Camera>& get_camera() const {
        return this->camera;
    }

    void set_camera(const Ref<Camera>& camera) {
        this->camera = camera;
    }

private:
    Ref<entt::registry> registry;
    Ref<Camera> camera;
};
