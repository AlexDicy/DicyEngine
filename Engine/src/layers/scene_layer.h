﻿#pragma once
#include "application.h"
#include "layer.h"
#include "scene/scene.h"

class SceneLayer : public Layer {
public:
    explicit SceneLayer(const Application* app);

    void update(const std::unique_ptr<Context>& ctx) override;

private:
    std::vector<Ref<VertexArray>> vertex_arrays;
    std::vector<Ref<VertexArray>> vertex_arrays_xyz;
    Ref<VertexArray> textured_square_vertex_array;
    Ref<Scene> scene;
    Ref<Camera> camera;
    Ref<Shader> shader;
    Ref<Shader> textured_shader;
    Ref<Texture> rgb_texture;
    Ref<Texture> rgba_texture;
};
