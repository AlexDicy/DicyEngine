#include "pch/enginepch.h"
#include "scene_layer.h"

#include "input/input.h"
#include "platforms/opengl/opengl_shader.h"
#include "rendering/camera/camera.h"
#include "rendering/camera/orthographic_camera.h"
#include "rendering/camera/perspective_camera.h"

#include <filesystem>
#include <glm/detail/type_quat.hpp>
#include <glm/ext/matrix_transform.hpp>

Ref<PerspectiveCamera> get_perspective_camera(const Ref<Window>&);
Ref<OrthographicCamera> get_orthographic_camera();

SceneLayer::SceneLayer(const Application* application) {
    const std::unique_ptr<Renderer>& renderer = application->get_renderer();
    const Ref<Window>& window = application->get_window();

    // square
    {
        Ref<VertexBuffer> vertex_buffer;
        Ref<IndexBuffer> index_buffer;
        constexpr float vertices[4 * 7] = {
            -0.5f, -0.5f, 2.1f, 0.1f, 0.1f, 0.7f, 1.0f, //
            0.5f,  -0.5f, 2.1f, 0.1f, 0.1f, 0.7f, 1.0f, //
            0.5f,  0.5f,  2.1f, 0.1f, 0.1f, 0.7f, 1.0f, //
            -0.5f, 0.5f,  2.1f, 0.1f, 0.1f, 0.7f, 1.0f, //
        };

        vertex_buffer = renderer->create_vertex_buffer(vertices, sizeof(vertices));

        vertex_buffer->set_layout({
            {DataType::FLOAT3, "position"},
            {DataType::FLOAT4, "color"},
        });

        constexpr unsigned int indexes[6] = {0, 1, 2, 2, 3, 0};
        index_buffer = renderer->create_index_buffer(indexes, 6);

        Ref<VertexArray> vertex_array;
        vertex_array = renderer->create_vertex_array(vertex_buffer, index_buffer);
        vertex_arrays.push_back(vertex_array);
    }

    // triangle
    {
        Ref<VertexBuffer> vertex_buffer;
        Ref<IndexBuffer> index_buffer;
        constexpr float vertices[3 * 7] = {
            -0.5f, -0.5f, 2.0f, 0.8f, 0.1f, 0.1f, 1.0f, //
            0.5f,  -0.5f, 2.0f, 0.8f, 0.1f, 0.1f, 1.0f, //
            0.0f,  0.5f,  2.0f, 0.1f, 0.9f, 0.1f, 1.0f, //
        };
        vertex_buffer = renderer->create_vertex_buffer(vertices, sizeof(vertices));

        vertex_buffer->set_layout({
            {DataType::FLOAT3, "position"},
            {DataType::FLOAT4, "color"},
        });

        constexpr unsigned int indexes[3] = {0, 1, 2};
        index_buffer = renderer->create_index_buffer(indexes, 3);

        Ref<VertexArray> vertex_array;
        vertex_array = renderer->create_vertex_array(vertex_buffer, index_buffer);
        vertex_arrays.push_back(vertex_array);
    }

    // x,y,z indicator
    {
        Ref<VertexBuffer> vertex_buffer_x;
        Ref<VertexBuffer> vertex_buffer_y;
        Ref<VertexBuffer> vertex_buffer_z;
        Ref<IndexBuffer> index_buffer;
        constexpr float vertices_x[3 * 7] = {
            0.0f, 0.0f, -0.1f, 0.8f, 0.1f, 0.1f, 0.8f, //
            0.0f, 0.0f, 0.1f,  0.8f, 0.1f, 0.1f, 0.8f, //
            1.0f, 0.0f, 0.0f,  0.8f, 0.1f, 0.1f, 0.8f, //
        };
        vertex_buffer_x = renderer->create_vertex_buffer(vertices_x, sizeof(vertices_x));
        constexpr float vertices_y[3 * 7] = {
            -0.1f, 0.0f, 0.0f, 0.1f, 0.1f, 0.8f, 0.8f, //
            0.1f,  0.0f, 0.0f, 0.1f, 0.1f, 0.8f, 0.8f, //
            0.0f,  1.0f, 0.0f, 0.1f, 0.1f, 0.8f, 0.8f, //
        };
        vertex_buffer_y = renderer->create_vertex_buffer(vertices_y, sizeof(vertices_y));
        constexpr float vertices_z[3 * 7] = {
            -0.1f, 0.0f, 0.0f, 0.1f, 0.8f, 0.1f, 0.8f, //
            0.1f,  0.0f, 0.0f, 0.1f, 0.8f, 0.1f, 0.8f, //
            0.0f,  0.0f, 1.0f, 0.1f, 0.8f, 0.1f, 0.8f, //
        };
        vertex_buffer_z = renderer->create_vertex_buffer(vertices_z, sizeof(vertices_z));

        vertex_buffer_x->set_layout({
            {DataType::FLOAT3, "position"},
            {DataType::FLOAT4, "color"},
        });
        vertex_buffer_y->set_layout({
            {DataType::FLOAT3, "position"},
            {DataType::FLOAT4, "color"},
        });
        vertex_buffer_z->set_layout({
            {DataType::FLOAT3, "position"},
            {DataType::FLOAT4, "color"},
        });

        constexpr unsigned int indexes[3] = {0, 1, 2};
        index_buffer = renderer->create_index_buffer(indexes, 3);

        Ref<VertexArray> vertex_array_x;
        Ref<VertexArray> vertex_array_y;
        Ref<VertexArray> vertex_array_z;
        vertex_array_x = renderer->create_vertex_array(vertex_buffer_x, index_buffer);
        vertex_array_y = renderer->create_vertex_array(vertex_buffer_y, index_buffer);
        vertex_array_z = renderer->create_vertex_array(vertex_buffer_z, index_buffer);
        vertex_arrays_xyz.push_back(vertex_array_x);
        vertex_arrays_xyz.push_back(vertex_array_y);
        vertex_arrays_xyz.push_back(vertex_array_z);
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

    this->shader = renderer->create_shader("../assets/shaders/flat.dshv", "../assets/shaders/flat.dshf");

    this->textured_shader = renderer->create_shader("../assets/shaders/textured.dshv", "../assets/shaders/textured.dshf");
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

    for (int x = -5; x <= 5; x++) {
        for (int y = -5; y <= 5; y++) {
            const glm::vec3 position = {x * 5.0f, y * 2.0f, abs(x) * 4.0f};
            const Rotation rotation = {x * 10.0f, 0.0f, 0.0f};
            const glm::mat4 transform = translate(glm::mat4(1.0f), position) * toMat4(rotation.to_quaternion());
            shader->upload_uniform_vec4("u_additional_color", glm::vec4((y + 5) / 10.0f, (x + 5) / 10.0f, 0.0f, 1.0f));

            for (const auto& vertex_array : vertex_arrays) {
                ctx->renderer->draw(vertex_array, this->shader, transform);
            }
        }
    }

    shader->upload_uniform_vec4("u_additional_color", glm::vec4(0.0f));
    constexpr glm::vec3 position = {0.0f, -1.0f, 1.2f};
    const glm::mat4 transform = translate(glm::mat4(1.0f), position);
    for (const auto& vertex_array : vertex_arrays_xyz) {
        ctx->renderer->draw(vertex_array, this->shader, transform);
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
