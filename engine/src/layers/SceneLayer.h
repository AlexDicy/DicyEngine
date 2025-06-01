#pragma once
#include "Layer.h"
#include "editor/scripts/EditorScript.h"
#include "scene/Scene.h"
#include "scene/lights/DirectionalLight.h"

class SceneLayer : public Layer {
public:
    explicit SceneLayer(const std::unique_ptr<Context>& ctx);

    void play(const std::unique_ptr<Context>& ctx) override;
    void update(const std::unique_ptr<Context>& ctx) override;
    void stop(const std::unique_ptr<Context>& ctx) override {}

private:
    std::vector<Ref<Entity>> addEntitiesForModels(const Ref<Renderer>& renderer, const std::string& path, glm::vec3 position, Rotation rotation = Rotation(),
                                                  glm::vec3 scale = glm::vec3(1.0f)) const;
    std::vector<Ref<Entity>> addEntitiesForModels(const Ref<Renderer>& renderer, const std::vector<Model>& models, glm::vec3 position, Rotation rotation = Rotation(),
                                                  glm::vec3 scale = glm::vec3(1.0f)) const;

    Ref<Scene> scene;
    Ref<Entity> cameraEntity;
    Ref<Shader> shader;
    Ref<SkyboxCube> skybox;
    Ref<DirectionalLight> directionalLight;
    Ref<Mesh> uiMesh;
    Ref<Shader> uiShader;
    Ref<EditorScript> editorScript;
    Ref<Shader> selectedEntityShared;
};
