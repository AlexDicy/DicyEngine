#pragma once
#include "Application.h"
#include "Layer.h"
#include "scene/Scene.h"
#include "scene/lights/DirectionalLight.h"
#include "scene/models/Model.h"

class SceneLayer : public Layer {
public:
    explicit SceneLayer(const Ref<Application>& app);

    void update(const std::unique_ptr<Context>& ctx) override;

private:
    void addEntitiesForModels(const Ref<Renderer>& renderer, const std::string& path, glm::vec3 position, Rotation rotation = Rotation(), glm::vec3 scale = glm::vec3(1.0f)) const;
    void addEntitiesForModels(const Ref<Renderer>& renderer, const std::vector<Model>& models, glm::vec3 position, Rotation rotation = Rotation(), glm::vec3 scale = glm::vec3(1.0f)) const;

    Ref<Scene> scene;
    Ref<Entity> cameraEntity;
    Ref<Shader> shader;
    Ref<SkyboxCube> skybox;
    Ref<DirectionalLight> directionalLight;
};
