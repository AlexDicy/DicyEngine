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
    std::vector<Model> xyz_models = ModelImporter::import_from_file(renderer, "../assets/models/arrows.glb");
    for (Model& arrow : xyz_models) {
        arrow.material.ignore_lighting = true;
    }
    this->add_entities_for_models(renderer, xyz_models, {0.0f, 0.0f, 0.0f});

    // camera
    this->camera_entity = this->scene->create_entity();
    // this->camera_entity->add<Transform>(glm::vec3(0.0f, 2.0f, 0.0f), Rotation(-15, 0, 0));
    this->camera_entity->add<Transform>(glm::vec3(0.0f, 15.0f, 0.0f), Rotation(-90, 0, 0));
    this->camera_entity->add<Script>(std::make_shared<CameraScript>(app, this->camera_entity));

    this->shader = app->get_shader_registry()->load("../assets/shaders/default-shader");
    Ref<Shader> skybox_shader = app->get_shader_registry()->load("../assets/shaders/skybox-shader");
    Ref<Texture2D> skybox_hdr = renderer->create_texture2d("../assets/skybox/kloofendal_48d_partly_cloudy_puresky_8k.hdr");
    Ref<Shader> equirectangular_to_cubemap_shader = app->get_shader_registry()->load("../assets/shaders/equirectangular-to-cubemap");
    Ref<TextureCube> skybox_texture_512 = renderer->create_texture_cube_from_hdr(skybox_hdr, equirectangular_to_cubemap_shader, 512);
    Ref<TextureCube> skybox_texture_2048 = renderer->create_texture_cube_from_hdr(skybox_hdr, equirectangular_to_cubemap_shader, 2048);
    this->irradiance_map = renderer->create_irradiance_map(skybox_texture_512, app->get_shader_registry()->load("../assets/shaders/cubemap-to-irradiance"), 512);
    this->skybox = std::make_shared<SkyboxCube>(renderer, skybox_shader, skybox_texture_2048);

    this->directional_light = std::make_shared<DirectionalLight>(Rotation(-70, 90, 0), 2.86f);
    Ref<Entity> light_entity = this->scene->create_entity();
    Ref<Entity> light_mesh_entity = this->scene->create_entity();
    light_entity->add<Script>(std::make_shared<LightScript>(app, light_entity, this->directional_light, light_mesh_entity));
    light_entity->add<Transform>();

    renderer->set_camera(this->scene->get_camera());

    this->add_entities_for_models(renderer, "../assets/models/sand_rocks/sand_rocks_small_01_1k.gltf", {0.0f, 0.0f, 3.0f});
    this->add_entities_for_models(renderer, "../assets/models/fountain.glb", {-4.0f, 0.2f, 4.0f});
    this->add_entities_for_models(renderer, "../assets/models/wooden_stool/wooden_stool_02_1k.gltf", {0.0f, 0.0f, 3.0f}, Rotation(), glm::vec3(4.0f));
    this->add_entities_for_models(renderer, "../assets/models/covered_car/covered_car_1k.gltf", {2.5f, 0.0f, 3.0f}, Rotation(0, -15, 0));
    this->add_entities_for_models(renderer, "../assets/models/picnic_table/wooden_picnic_table_1k.gltf", {0.0f, 0.0f, 6.0f}, Rotation(4, -85, 0));

    {
        Model model = ModelImporter::import_from_file(renderer, "../assets/models/lamp.glb")[0];
        const VertexData* vertex_data = model.vertices.data();
        auto vertex_data_floats = reinterpret_cast<const float*>(vertex_data);
        light_mesh_entity->add<Mesh>(renderer, vertex_data_floats, model.vertices.size() * sizeof(VertexData), model.indexes.data(), model.indexes.size(), model.material,
                                     model.transformation_matrix);
        light_mesh_entity->add<Transform>(glm::vec3(0.0, 3.0f, 0.0f), Rotation(), glm::vec3(1.0f));
    }

    Model sphere_model = std::move(ModelImporter::import_from_file(renderer, "../assets/models/sphere.glb")[0]);
    for (int x = 0; x < 10; x++) {
        for (int z = 0; z < 10; z++) {
            const float x_pos = -5.0f + x + 0.5f;
            const float z_pos = -5.0f + z + 0.5f;
            const VertexData* vertex_data = sphere_model.vertices.data();
            auto vertex_data_floats = reinterpret_cast<const float*>(vertex_data);
            unsigned char roughness = static_cast<unsigned char>(static_cast<float>(9 - x) / 9 * 255.0f);
            unsigned char metallic = static_cast<unsigned char>(static_cast<float>(9 - z) / 9 * 255.0f);
            Material material(renderer->create_texture2d(4, 1, 1, std::array<unsigned char, 4>{250, 40, 40, 255}.data()),
                              renderer->create_texture2d(3, 1, 1, std::array<unsigned char, 3>{255, roughness, metallic}.data()));
            Ref<Entity> entity = this->scene->create_entity();
            entity->add<Mesh>(renderer, vertex_data_floats, sphere_model.vertices.size() * sizeof(VertexData), sphere_model.indexes.data(), sphere_model.indexes.size(), material,
                              sphere_model.transformation_matrix);
            entity->add<Transform>(glm::vec3(x_pos, 4.0f, z_pos), Rotation(), glm::vec3(0.4f));
        }
    }

    // point lights
    for (glm::vec3 positions[] = {{1.0f, 1.0f, 1.0f}, {4.0f, 4.2f, 2.0f}}; const glm::vec3& position : positions) {
        Ref<Entity> point_light_entity = this->scene->create_entity();
        point_light_entity->add<PointLight>(position, glm::vec3(0.2f, 0.2f, 1.0f), 10.0f);
        point_light_entity->add<Transform>(position, Rotation(), glm::vec3(0.1f));
        const VertexData* vertex_data = sphere_model.vertices.data();
        auto vertex_data_floats = reinterpret_cast<const float*>(vertex_data);
        auto material = Material(renderer->create_texture2d(4, 1, 1, std::array<unsigned char, 4>{50, 50, 255, 255}.data()));
        material.ignore_lighting = true;
        point_light_entity->add<Mesh>(renderer, vertex_data_floats, sphere_model.vertices.size() * sizeof(VertexData), sphere_model.indexes.data(), sphere_model.indexes.size(),
                                      material, sphere_model.transformation_matrix);
    }
}

void SceneLayer::update(const std::unique_ptr<Context>& ctx) {
    DE_PROFILE_FUNCTION();
    ctx->renderer->begin_frame();

    // ambient lighting
    ctx->renderer->prepare_ambient_light(this->irradiance_map);
    // add point lights to the renderer
    const auto point_lights_view = this->scene->get_point_lights();
    for (const auto& entity : point_lights_view) {
        PointLight& light = point_lights_view.get<PointLight>(entity);
        ctx->renderer->add_point_light(light);
    }

    // render meshes
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

    // render skybox
    ctx->renderer->draw_skybox(this->skybox);

    ctx->renderer->end_frame();

    const auto scripts_view = this->scene->get_scripts();
    for (const auto& entity : scripts_view) {
        Script& script = scripts_view.get<Script>(entity);
        script.entity_script->on_update(ctx->delta_time);
    }
}

void SceneLayer::add_entities_for_models(const Ref<Renderer>& renderer, const std::string& path, const glm::vec3 position, const Rotation rotation, const glm::vec3 scale) const {
    const std::vector<Model> models = ModelImporter::import_from_file(renderer, path);
    this->add_entities_for_models(renderer, models, position, rotation, scale);
}

void SceneLayer::add_entities_for_models(const Ref<Renderer>& renderer, const std::vector<Model>& models, const glm::vec3 position, const Rotation rotation,
                                         const glm::vec3 scale) const {
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
