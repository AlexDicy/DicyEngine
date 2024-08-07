﻿#include "pch/enginepch.h"
#include "scene_layer.h"

#include "input/input.h"
#include "platforms/opengl/opengl_shader.h"
#include "rendering/shader_registry.h"
#include "rendering/camera/camera.h"
#include "rendering/camera/orthographic_camera.h"
#include "rendering/camera/perspective_camera.h"
#include "scene/components/mesh.h"
#include "scene/components/transform.h"

#include <filesystem>
#include <glm/detail/type_quat.hpp>
#include <glm/ext/matrix_transform.hpp>

Ref<PerspectiveCamera> get_perspective_camera(const Ref<Window>&);
Ref<OrthographicCamera> get_orthographic_camera();

SceneLayer::SceneLayer(const Application* app) {
    const Ref<Renderer>& renderer = app->get_renderer();
    const Ref<Window>& window = app->get_window();
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

    // camera
    this->camera = get_perspective_camera(window);

    this->shader = app->get_shader_registry()->load("../assets/shaders/flat");

    this->textured_shader = app->get_shader_registry()->load("../assets/shaders/textured");
    this->textured_shader->upload_uniform_int("u_texture", 0);
    this->rgb_texture = renderer->create_texture2d("../assets/dicystudios_rgb.png");
    this->rgba_texture = renderer->create_texture2d("../assets/dicystudios_rgba.png");

    Input::set_action("change_camera", InputCode::KEY_O);
    Input::set_action("move_camera_up", InputCode::KEY_E);
    Input::set_action("move_camera_down", InputCode::KEY_Q);

    constexpr float sensitivity = 0.16f;
    Input::bind_axis("look_x", [this](const float delta_x) {
        if (Input::is_action_pressed("right_click")) {
            this->camera->set_yaw(this->camera->get_yaw() + delta_x * sensitivity);
        }
    });
    Input::bind_axis("look_y", [this](const float delta_y) {
        if (Input::is_action_pressed("right_click")) {
            const float pitch = glm::clamp(this->camera->get_pitch() + delta_y * sensitivity, -90.0f, 90.0f);
            this->camera->set_pitch(pitch);
        }
    });
    Input::bind_action_pressed("change_camera", [this, &renderer, &window] {
        const glm::vec3 position = this->camera->get_position();
        const Rotation rotation = this->camera->get_rotation();
        static bool camera_bool = false;
        if (camera_bool) {
            this->camera = get_perspective_camera(window);
        } else {
            this->camera = get_orthographic_camera();
        }
        renderer->set_camera(camera);
        camera_bool = !camera_bool;
        this->camera->set_position(position);
        this->camera->set_rotation(rotation);
    });

    renderer->set_camera(camera);
}

constexpr float camera_speed = 2.0f;

void SceneLayer::update(const std::unique_ptr<Context>& ctx) {
    DE_PROFILE_FUNCTION();
    const float camera_speed_delta = camera_speed * ctx->delta_time;
    auto camera_position = this->camera->get_position();
    const float yaw = glm::radians(this->camera->get_yaw());
    const auto forward = glm::vec3(sinf(yaw), 0, cosf(yaw));
    const auto right = glm::vec3(forward.z, 0, -forward.x);
    if (Input::is_action_pressed("move_left")) {
        camera_position -= right * camera_speed_delta;
    }
    if (Input::is_action_pressed("move_right")) {
        camera_position += right * camera_speed_delta;
    }
    if (Input::is_action_pressed("move_forward")) {
        camera_position += forward * camera_speed_delta;
    }
    if (Input::is_action_pressed("move_backward")) {
        camera_position -= forward * camera_speed_delta;
    }
    if (Input::is_action_pressed("move_camera_up")) {
        camera_position.y += camera_speed_delta;
    }
    if (Input::is_action_pressed("move_camera_down")) {
        camera_position.y -= camera_speed_delta;
    }
    this->camera->set_position(camera_position);

    ctx->renderer->begin_frame();

    const auto meshes_view = this->scene->get_meshes();


    for (const auto& entity : meshes_view) {
        const Transform& transform = meshes_view.get<Transform>(entity);
        const Mesh& mesh = meshes_view.get<Mesh>(entity);

        const glm::mat4 transform_mat = translate(glm::mat4(1.0f), transform.position) * toMat4(transform.rotation.to_quaternion());
        ctx->renderer->draw(mesh.vertex_array, this->shader, transform_mat);
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
}

Ref<PerspectiveCamera> get_perspective_camera(const Ref<Window>& window) {
    return std::make_shared<PerspectiveCamera>(90.0f, static_cast<float>(window->get_width()) / static_cast<float>(window->get_height()));
}

Ref<OrthographicCamera> get_orthographic_camera() {
    return std::make_shared<OrthographicCamera>(-1.6f, 1.6f, -0.9f, 0.9f);
}
