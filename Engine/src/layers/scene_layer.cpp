#include "pch/enginepch.h"
#include "scene_layer.h"

#include <filesystem>
#include <glm/ext/matrix_transform.hpp>

#include "editor/scripts/camera_script.h"
#include "editor/scripts/light_script.h"
#include "scene/models/model_importer.h"

SceneLayer::SceneLayer(const Ref<Application>& app) {
    const Ref<Renderer>& renderer = app->get_renderer();
    this->scene = std::make_shared<Scene>();

    // x,y,z indicator
    {
        const auto x = this->scene->create_entity();
        const auto y = this->scene->create_entity();
        const auto z = this->scene->create_entity();
        constexpr glm::vec3 position = {0.0f, 0.0f, 0.0f};
        x->add<Transform>(position);
        y->add<Transform>(position);
        z->add<Transform>(position);
        constexpr float vertices_x[3 * 8] = {
            0.0f, 0.0f, -0.1f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, //
            0.0f, 0.0f, 0.1f,  0.0f, 1.0f, 0.0f, 0.0f, 0.0f, //
            1.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f, 0.0f, 0.0f, //
        };
        constexpr float vertices_y[3 * 8] = {
            -0.1f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, //
            0.1f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, //
            0.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, //
        };
        constexpr float vertices_z[3 * 8] = {
            -0.1f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, //
            0.1f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, //
            0.0f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, //
        };
        constexpr unsigned int indexes[3] = {0, 1, 2};
        x->add<Mesh>(renderer, vertices_x, sizeof(vertices_x), indexes, 3, Material(renderer->create_texture2d(4, 1, 1, std::array<unsigned char, 4>{204, 24, 24, 204}.data())));
        y->add<Mesh>(renderer, vertices_y, sizeof(vertices_y), indexes, 3, Material(renderer->create_texture2d(4, 1, 1, std::array<unsigned char, 4>{24, 24, 204, 204}.data())));
        z->add<Mesh>(renderer, vertices_z, sizeof(vertices_z), indexes, 3, Material(renderer->create_texture2d(4, 1, 1, std::array<unsigned char, 4>{24, 204, 24, 204}.data())));
    }

    // camera
    this->camera = this->scene->create_entity();
    // this->camera->add<Transform>(glm::vec3(0.0f, 2.0f, 0.0f), Rotation(-15, 0, 0));
    this->camera->add<Transform>(glm::vec3(0.0f, 10.0f, 0.0f), Rotation(-90, 0, 0));
    this->camera_script = std::make_shared<CameraScript>(app, this->camera);
    this->camera->add<Script>(camera_script);

    this->shader = app->get_shader_registry()->load("../assets/shaders/default-shader");

    this->directional_light = std::make_shared<DirectionalLight>(Rotation(-70, 90, 0), 2.86f);
    Ref<Entity> light_entity = this->scene->create_entity();
    Ref<Entity> light_mesh_entity = this->scene->create_entity();
    light_entity->add<Script>(std::make_shared<LightScript>(app, light_entity, this->directional_light, light_mesh_entity));
    light_entity->add<Transform>();
    constexpr float light_vertices[4 * 8] = {
        -0.2f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, //
        0.2f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, //
        0.2f,  0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, //
        -0.2f, 0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, //
    };
    constexpr unsigned int indexes[6] = {0, 1, 2, 2, 3, 0};
    light_entity->add<Mesh>(renderer, light_vertices, sizeof(light_vertices), indexes, 6,
                            Material(renderer->create_texture2d(4, 1, 1, std::array<unsigned char, 4>{255, 255, 255, 220}.data())));

    renderer->set_camera(this->camera_script->get_camera());

    this->load_model(renderer, "../assets/models/sand_rocks/sand_rocks_small_01_1k.gltf", {0.0f, 0.0f, 3.0f});
    this->load_model(renderer, "../assets/models/fountain.glb", {-4.0f, 0.2f, 4.0f});
    this->load_model(renderer, "../assets/models/wooden_stool/wooden_stool_02_1k.gltf", {0.0f, 0.0f, 3.0f}, Rotation(), glm::vec3(4.0f));
    this->load_model(renderer, "../assets/models/covered_car/covered_car_1k.gltf", {2.5f, 0.0f, 3.0f}, Rotation(0, -15, 0));
    this->load_model(renderer, "../assets/models/picnic_table/wooden_picnic_table_1k.gltf", {0.0f, 0.0f, 6.0f}, Rotation(4, -85, 0));

    {
        Model model = ModelImporter::import_from_file(renderer, "../assets/models/lamp.glb")[0];
        const VertexData* vertex_data = model.vertices.data();
        auto vertex_data_floats = reinterpret_cast<const float*>(vertex_data);
        light_mesh_entity->add<Mesh>(renderer, vertex_data_floats, model.vertices.size() * sizeof(VertexData), model.indexes.data(), model.indexes.size(), model.material,
                                     model.transformation_matrix);
        light_mesh_entity->add<Transform>(glm::vec3(0.0, 3.0f, 0.0f), Rotation(), glm::vec3(1.0f));
    }

    std::vector<Model> sphere_models = ModelImporter::import_from_file(renderer, "../assets/models/sphere.glb");
    for (int x = 0; x < 10; x++) {
        for (int z = 0; z < 10; z++) {
            const float x_pos = -5.0f + x + 0.5f;
            const float z_pos = -5.0f + z + 0.5f;
            for (const Model& model : sphere_models) {
                const VertexData* vertex_data = model.vertices.data();
                auto vertex_data_floats = reinterpret_cast<const float*>(vertex_data);
                unsigned char roughness = static_cast<unsigned char>(static_cast<float>(9 - x) / 9 * 255.0f);
                unsigned char metallic = static_cast<unsigned char>(static_cast<float>(9 - z) / 9 * 255.0f);
                Material material(renderer->create_texture2d(4, 1, 1, std::array<unsigned char, 4>{250, 40, 40, 255}.data()),
                                  renderer->create_texture2d(3, 1, 1, std::array<unsigned char, 3>{255, roughness, metallic}.data()));
                Ref<Entity> entity = this->scene->create_entity();
                entity->add<Mesh>(renderer, vertex_data_floats, model.vertices.size() * sizeof(VertexData), model.indexes.data(), model.indexes.size(), material,
                                  model.transformation_matrix);
                entity->add<Transform>(glm::vec3(x_pos, 4.0f, z_pos), Rotation(), glm::vec3(0.4f));
            }
        }
    }
}

void SceneLayer::update(const std::unique_ptr<Context>& ctx) {
    DE_PROFILE_FUNCTION();
    ctx->renderer->begin_frame();

    const auto meshes_view = this->scene->get_meshes();
    for (const auto& entity : meshes_view) {
        const Transform& transform = meshes_view.get<Transform>(entity);
        const Mesh& mesh = meshes_view.get<Mesh>(entity);

        const glm::mat4 translation_mat = translate(glm::mat4(1.0f), transform.position);
        const glm::mat4 rotation_mat = toMat4(transform.rotation.to_quaternion());
        const glm::mat4 scale_mat = scale(translation_mat * rotation_mat, transform.scale);
        const glm::mat4 transform_mat = scale_mat * mesh.transformation_matrix;
        if (mesh.material.albedo) {
            ctx->renderer->draw(mesh.vertex_array, this->shader, transform_mat, this->directional_light, mesh.material);
        } else {
            ctx->renderer->draw(mesh.vertex_array, this->shader, transform_mat, this->directional_light);
        }
    }

    ctx->renderer->end_frame();

    const auto scripts_view = this->scene->get_scripts();
    for (const auto& entity : scripts_view) {
        Script& script = scripts_view.get<Script>(entity);
        script.entity_script->on_update(ctx->delta_time);
    }
}

void SceneLayer::load_model(const Ref<Renderer>& renderer, const std::string& path, const glm::vec3 position, const Rotation rotation, const glm::vec3 scale) const {
    std::vector<Model> models = ModelImporter::import_from_file(renderer, path);
    for (const Model& model : models) {
        const VertexData* vertex_data = model.vertices.data();
        auto vertex_data_floats = reinterpret_cast<const float*>(vertex_data);
        const Material& material = model.material;
        Ref<Entity> entity = this->scene->create_entity();
        entity->add<Mesh>(renderer, vertex_data_floats, model.vertices.size() * sizeof(VertexData), model.indexes.data(), model.indexes.size(), material,
                          model.transformation_matrix);
        entity->add<Transform>(position, rotation, scale);
    }
}
