#pragma once
#include "rendering/camera/camera.h"
#include "scene/entities/entity_script.h"

class CameraScript final : public EntityScript {
public:
    explicit CameraScript(const Application* app, const Ref<Entity>& entity);

    void on_update(float delta_time) override;

    Ref<Camera>& get_camera() {
        return this->camera;
    }

private:
    Ref<Camera> camera;
    Transform* transform;
    glm::vec3 velocity = glm::vec3(0.0f);
    bool move_faster;
};
