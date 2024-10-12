#pragma once
#include "application.h"
#include "layer.h"
#include "scene/scene.h"
#include "scene/skybox_cube.h"
#include "scene/lights/directional_light.h"
#include "scene/models/model.h"

class SceneLayer : public Layer {
public:
    explicit SceneLayer(const Ref<Application>& app);

    void update(const std::unique_ptr<Context>& ctx) override;

private:
    void add_entities_for_models(const Ref<Renderer>& renderer, const std::string& path, glm::vec3 position, Rotation rotation = Rotation(), glm::vec3 scale = glm::vec3(1.0f)) const;
    void add_entities_for_models(const Ref<Renderer>& renderer, const std::vector<Model>& models, glm::vec3 position, Rotation rotation = Rotation(), glm::vec3 scale = glm::vec3(1.0f)) const;

    Ref<Scene> scene;
    Ref<Entity> camera_entity;
    Ref<Shader> shader;
    Ref<SkyboxCube> skybox;
    Ref<DirectionalLight> directional_light;
};
