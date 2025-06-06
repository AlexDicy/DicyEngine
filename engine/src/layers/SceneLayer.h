#pragma once
#include "Layer.h"

class SceneLayer : public Layer {
public:
    explicit SceneLayer(const std::unique_ptr<Context>& ctx);

    void play(const std::unique_ptr<Context>& ctx) override;
    void update(const std::unique_ptr<Context>& ctx) override;
    void stop(const std::unique_ptr<Context>& ctx) override {}

private:
    Ref<Entity> cameraEntity;
    Ref<Shader> shader;
    Ref<SkyboxCube> skybox;
    Ref<DirectionalLight> directionalLight;
};
