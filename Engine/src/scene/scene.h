#pragma once
#include "entities/entity.h"
#include "components/mesh.h"
#include "components/script.h"
#include "components/transform.h"
#include "entt/entt.hpp"

class Scene {
public:
    Scene();

    Ref<Entity> create_entity();

    auto get_meshes() const {
        return this->registry->view<Mesh, Transform>();
    }

    auto get_scripts() const {
        return this->registry->view<Script>();
    }

private:
    Ref<entt::registry> registry;
};
