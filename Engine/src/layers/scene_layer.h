#pragma once
#include "application.h"
#include "layer.h"
#include "editor/scripts/camera_script.h"
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
    Ref<Entity> camera;
    Ref<CameraScript> camera_script;
    Ref<Shader> shader;
    Ref<Shader> textured_shader;
    Ref<Texture> rgb_texture;
    Ref<Texture> rgba_texture;
};


class MovingSquareScript final : public EntityScript {
public:
    MovingSquareScript(const Application* app, const Ref<Entity>& entity): EntityScript(app, entity) {}

    void on_update(float delta_time) override;

private:
    float move_index = 0;
};
