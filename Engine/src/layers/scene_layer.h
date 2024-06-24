#pragma once
#include "application.h"
#include "layer.h"

class SceneLayer : public Layer {
public:
    explicit SceneLayer(const Application* application);

    void update(const std::unique_ptr<Renderer>& renderer) override;

private:
    std::vector<std::shared_ptr<VertexArray>> vertex_arrays;
    std::shared_ptr<Camera> camera;
    std::shared_ptr<Shader> shader;
};
