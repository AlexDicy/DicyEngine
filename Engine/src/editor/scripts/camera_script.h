#pragma once
#include "rendering/camera/camera.h"
#include "scene/entities/entity_script_java.h"

class CameraScript final : public EntityScript {
public:
    explicit CameraScript(const Application* app, const Ref<Entity>& entity);

    void on_update(float delta_time) override;
    void on_spawn() override;
    void on_destroy() override;
    void on_awake() override;
    void on_sleep() override;

    Ref<Camera>& get_camera() {
        return this->camera;
    }

private:
    Ref<EntityScriptJava> script;
    Ref<Camera> camera;
    Transform* transform;
    glm::vec3 velocity = glm::vec3(0.0f);
    bool move_faster;
};
