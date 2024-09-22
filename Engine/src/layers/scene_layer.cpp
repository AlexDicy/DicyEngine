#include "pch/enginepch.h"
#include "scene_layer.h"

#include <filesystem>
#include <glm/ext/matrix_transform.hpp>

#include "editor/scripts/camera_script.h"
#include "platforms/opengl/opengl_shader.h"
#include "scene/models/model_importer.h"

SceneLayer::SceneLayer(const Application* app) {
    const Ref<Renderer>& renderer = app->get_renderer();
    this->scene = std::make_shared<Scene>();

    for (int x = -5; x <= 5; x++) {
        for (int y = -5; y <= 5; y++) {
            const glm::vec3 position = {x * 5.0f, y * 2.0f, abs(x) * 4.0f};
            const Rotation rotation = {x * 10.0f, 0.0f, 0.0f};
            // square
            {
                const auto square = this->scene->create_entity();
                square->add<Transform>(position, rotation);
                constexpr float vertices[4 * 7] = {
                    -0.5f, -0.5f, 2.1f, 0.1f, 0.1f, 0.7f, 1.0f, //
                    0.5f,  -0.5f, 2.1f, 0.1f, 0.1f, 0.7f, 1.0f, //
                    0.5f,  0.5f,  2.1f, 0.1f, 0.1f, 0.7f, 1.0f, //
                    -0.5f, 0.5f,  2.1f, 0.1f, 0.1f, 0.7f, 1.0f, //
                };
                constexpr unsigned int indexes[6] = {0, 1, 2, 2, 3, 0};
                square->add<Mesh>(renderer, vertices, sizeof(vertices), indexes, 6);
            }

            // triangle
            {
                const auto triangle = this->scene->create_entity();
                triangle->add<Transform>(position, rotation);
                constexpr float vertices[3 * 7] = {
                    -0.5f, -0.5f, 2.0f, 0.8f, 0.1f, 0.1f, 1.0f, //
                    0.5f,  -0.5f, 2.0f, 0.8f, 0.1f, 0.1f, 1.0f, //
                    0.0f,  0.5f,  2.0f, 0.1f, 0.9f, 0.1f, 1.0f, //
                };
                constexpr unsigned int indexes[3] = {0, 1, 2};
                triangle->add<Mesh>(renderer, vertices, sizeof(vertices), indexes, 3);
            }
        }
    }

    // x,y,z indicator
    {
        const auto x = this->scene->create_entity();
        const auto y = this->scene->create_entity();
        const auto z = this->scene->create_entity();
        constexpr glm::vec3 position = {0.0f, -1.0f, 1.2f};
        x->add<Transform>(position);
        y->add<Transform>(position);
        z->add<Transform>(position);
        constexpr float vertices_x[3 * 7] = {
            0.0f, 0.0f, -0.1f, 0.8f, 0.1f, 0.1f, 0.8f, //
            0.0f, 0.0f, 0.1f,  0.8f, 0.1f, 0.1f, 0.8f, //
            1.0f, 0.0f, 0.0f,  0.8f, 0.1f, 0.1f, 0.8f, //
        };
        constexpr float vertices_y[3 * 7] = {
            -0.1f, 0.0f, 0.0f, 0.1f, 0.1f, 0.8f, 0.8f, //
            0.1f,  0.0f, 0.0f, 0.1f, 0.1f, 0.8f, 0.8f, //
            0.0f,  1.0f, 0.0f, 0.1f, 0.1f, 0.8f, 0.8f, //
        };
        constexpr float vertices_z[3 * 7] = {
            -0.1f, 0.0f, 0.0f, 0.1f, 0.8f, 0.1f, 0.8f, //
            0.1f,  0.0f, 0.0f, 0.1f, 0.8f, 0.1f, 0.8f, //
            0.0f,  0.0f, 1.0f, 0.1f, 0.8f, 0.1f, 0.8f, //
        };
        constexpr unsigned int indexes[3] = {0, 1, 2};
        x->add<Mesh>(renderer, vertices_x, sizeof(vertices_x), indexes, 3);
        y->add<Mesh>(renderer, vertices_y, sizeof(vertices_y), indexes, 3);
        z->add<Mesh>(renderer, vertices_z, sizeof(vertices_z), indexes, 3);
    }

    // textured square
    {
        Ref<VertexBuffer> vertex_buffer;
        Ref<IndexBuffer> index_buffer;
        constexpr float vertices[4 * 5] = {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, //
            0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, //
            0.5f,  0.5f,  0.0f, 1.0f, 1.0f, //
            -0.5f, 0.5f,  0.0f, 0.0f, 1.0f, //
        };

        vertex_buffer = renderer->create_vertex_buffer(vertices, sizeof(vertices));

        vertex_buffer->set_layout({
            {DataType::FLOAT3, "position"},
            {DataType::FLOAT2, "texture_coords"},
        });

        constexpr unsigned int indexes[6] = {0, 1, 2, 2, 3, 0};
        index_buffer = renderer->create_index_buffer(indexes, 6);

        textured_square_vertex_array = renderer->create_vertex_array(vertex_buffer, index_buffer);
    }

    // moving square
    {
        const auto square = this->scene->create_entity();
        square->add<Transform>(glm::vec3({0.0f, 0.0f, 4.0f}), Rotation(), glm::vec3(2.46f));
        constexpr float vertices[4 * 7] = {
            -0.5f, -0.5f, 0.0f, 0.1f, 0.7f, 0.1f, 1.0f, //
            0.5f, -0.5f, 0.0f, 0.1f, 0.7f, 0.1f, 1.0f, //
            0.5f, 0.5f, 0.0f, 0.1f, 0.7f, 0.1f, 1.0f, //
            -0.5f, 0.5f, 0.0f, 0.1f, 0.7f, 0.1f, 1.0f, //
        };
        constexpr unsigned int indexes[6] = {0, 1, 2, 2, 3, 0};
        square->add<Mesh>(renderer, vertices, sizeof(vertices), indexes, 6);
        square->add<Script>(std::make_shared<MovingSquareScript>(app, square));
    }

    // camera
    this->camera = this->scene->create_entity();
    this->camera->add<Transform>();
    this->camera_script = std::make_shared<CameraScript>(app, this->camera);
    this->camera->add<Script>(camera_script);

    this->shader = app->get_shader_registry()->load("../assets/shaders/flat");

    this->textured_shader = app->get_shader_registry()->load("../assets/shaders/textured");
    this->textured_shader->upload_uniform_int("u_texture", 0);
    this->rgb_texture = renderer->create_texture2d("../assets/dicystudios_rgb.png");
    this->rgba_texture = renderer->create_texture2d("../assets/dicystudios_rgba.png");

    renderer->set_camera(this->camera_script->get_camera());

    ModelImporter::import_from_file("../assets/models/fountain.glb");
}

void SceneLayer::update(const std::unique_ptr<Context>& ctx) {
    DE_PROFILE_FUNCTION();
    ctx->renderer->begin_frame();

    const auto meshes_view = this->scene->get_meshes();
    for (const auto& entity : meshes_view) {
        const Transform& transform = meshes_view.get<Transform>(entity);
        const Mesh& mesh = meshes_view.get<Mesh>(entity);

        const glm::mat4 transform_mat = translate(glm::mat4(1.0f), transform.position) * toMat4(transform.rotation.to_quaternion());
        ctx->renderer->draw(mesh.vertex_array, this->shader, scale(transform_mat, transform.scale));
    }

    const glm::mat4 square_rgb_transform = translate(glm::mat4(1.0f), {-1.0f, 0.6f, 1.2f});
    const glm::mat4 square_rgba_transform = translate(glm::mat4(1.0f), {-1.0f, -0.6f, 1.2f});
    this->textured_shader->bind();
    // rgb
    this->rgb_texture->bind(0);
    ctx->renderer->draw(textured_square_vertex_array, this->textured_shader, square_rgb_transform);
    // rgba
    this->rgba_texture->bind(0);
    ctx->renderer->draw(textured_square_vertex_array, this->textured_shader, square_rgba_transform);

    ctx->renderer->end_frame();

    const auto scripts_view = this->scene->get_scripts();
    for (const auto& entity : scripts_view) {
        Script& script = scripts_view.get<Script>(entity);
        script.entity_script->on_update(ctx->delta_time);
    }
}

void MovingSquareScript::on_update(float delta_time) {
    this->move_index = this->move_index + 1 * delta_time;
    const float y = glm::cos(this->move_index) * 5;
    this->get_component<Transform>().position.y = y;
}
