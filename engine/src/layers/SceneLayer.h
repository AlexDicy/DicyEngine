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
    Ref<Scene> scene;
    Ref<Entity> cameraEntity;
    Ref<Shader> shader;
    Ref<SkyboxCube> skybox;
    Ref<DirectionalLight> directionalLight;
    Ref<Mesh> uiMesh;
    Ref<Shader> uiShader;
    Ref<EditorScript> editorScript;
};
