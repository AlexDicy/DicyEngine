#pragma once
#include "application.h"
#include "layer.h"
#include "editor/scripts/camera_script.h"
#include "scene/scene.h"
#include "scene/lights/directional_light.h"

class SceneLayer : public Layer {
public:
    explicit SceneLayer(const Application* app);

    void update(const std::unique_ptr<Context>& ctx) override;

private:
    void load_model(const Ref<Renderer>& renderer, const std::string& path, glm::vec3 position, Rotation rotation = Rotation(), glm::vec3 scale = glm::vec3(1.0f)) const;

    Ref<Scene> scene;
    Ref<Entity> camera;
    Ref<CameraScript> camera_script;
    Ref<Shader> shader;
    Ref<DirectionalLight> directional_light;
};


class MovingSquareScript final : public EntityScript {
public:
    MovingSquareScript(const Application* app, const Ref<Entity>& entity) : EntityScript(app, entity) {}

    void on_update(float delta_time) override;

private:
    float move_index = 0;
};
