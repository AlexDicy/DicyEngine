#pragma once
#include "entity.h"
#include "components/mesh.h"
#include "components/transform.h"
#include "entt/entt.hpp"

class Scene {
public:
    Scene();

    Ref<Entity> create_entity();

    auto get_meshes() const {
        return this->registry->view<Mesh, Transform>();
    }

private:
    Ref<entt::registry> registry;
};
