#pragma once
#include "application.h"
#include "layer.h"
#include "editor/scripts/camera_script.h"
#include "scene/scene.h"
#include "scene/skybox.h"
#include "scene/lights/directional_light.h"

class SceneLayer : public Layer {
public:
    explicit SceneLayer(const Ref<Application>& app);

    void update(const std::unique_ptr<Context>& ctx) override;

private:
    void load_model(const Ref<Renderer>& renderer, const std::string& path, glm::vec3 position, Rotation rotation = Rotation(), glm::vec3 scale = glm::vec3(1.0f)) const;

    Ref<Scene> scene;
    Ref<Entity> camera_entity;
    Ref<Shader> shader;
    Ref<Skybox> skybox;
    Ref<DirectionalLight> directional_light;
};
