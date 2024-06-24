#include "pch/enginepch.h"
#include "scene_layer.h"

#include "input/input.h"
#include "platforms/opengl/opengl_shader.h"

#include <glm/ext/matrix_transform.hpp>

SceneLayer::SceneLayer(const Application* application) {
    const std::unique_ptr<Renderer>& renderer = application->get_renderer();

    // square
    {
        std::shared_ptr<VertexBuffer> vertex_buffer;
        std::shared_ptr<IndexBuffer> index_buffer;
        constexpr float vertices[4 * 7] = {
            -0.5f, -0.5f, 0.0f, 0.1f, 0.1f, 0.7f, 1.0f, //
            0.5f,  -0.5f, 0.0f, 0.1f, 0.1f, 0.7f, 1.0f, //
            0.5f,  0.5f,  0.0f, 0.1f, 0.1f, 0.7f, 1.0f, //
            -0.5f, 0.5f,  0.0f, 0.1f, 0.1f, 0.7f, 1.0f, //
        };

        vertex_buffer.reset(renderer->create_vertex_buffer(vertices, sizeof(vertices)));

        vertex_buffer->set_layout({
            {DataType::FLOAT3, "position"},
            {DataType::FLOAT4, "color"},
        });

        constexpr unsigned int indexes[6] = {0, 1, 2, 2, 3, 0};
        index_buffer.reset(renderer->create_index_buffer(indexes, 6));

        std::shared_ptr<VertexArray> vertex_array;
        vertex_array.reset(renderer->create_vertex_array(vertex_buffer, index_buffer));
        vertex_arrays.push_back(vertex_array);
    }

    // triangle
    {
        std::shared_ptr<VertexBuffer> vertex_buffer;
        std::shared_ptr<IndexBuffer> index_buffer;
        constexpr float vertices[3 * 7] = {
            -0.5f, -0.5f, 0.0f, 0.8f, 0.1f, 0.1f, 1.0f, //
            0.5f,  -0.5f, 0.0f, 0.8f, 0.1f, 0.1f, 1.0f, //
            0.0f,  0.5f,  0.0f, 0.1f, 0.9f, 0.1f, 1.0f, //
        };
        vertex_buffer.reset(renderer->create_vertex_buffer(vertices, sizeof(vertices)));

        vertex_buffer->set_layout({
            {DataType::FLOAT3, "position"},
            {DataType::FLOAT4, "color"},
        });

        constexpr unsigned int indexes[3] = {0, 1, 2};
        index_buffer.reset(renderer->create_index_buffer(indexes, 3));

        std::shared_ptr<VertexArray> vertex_array;
        vertex_array.reset(renderer->create_vertex_array(vertex_buffer, index_buffer));
        vertex_arrays.push_back(vertex_array);
    }

    // camera
    this->camera = std::make_shared<Camera>(-1.6f, 1.6f, -0.9f, 0.9f);

    const std::string vertex_source = R"(
        #version 330 core

        layout(location = 0) in vec3 position;
        layout(location = 1) in vec4 color;

        uniform mat4 u_view_projection;
        uniform mat4 u_transform;

        out vec4 v_color;

        void main() {
            gl_Position = u_view_projection * u_transform * vec4(position, 1.0);
            v_color = color;
        }
    )";

    const std::string fragment_source = R"(
        #version 330 core

        layout(location = 0) out vec4 color;

        in vec4 v_color;

        void main() {
            color = v_color;
        }
    )";
    shader.reset(new OpenGLShader(vertex_source, fragment_source));
}

constexpr float camera_speed = 2.0f;

void SceneLayer::update(const std::unique_ptr<Context>& ctx) {
    const float camera_speed_delta = camera_speed * ctx->delta_time;
    auto camera_position = this->camera->get_position();
    if (Input::is_action_pressed("move_left")) {
        camera_position.x -= camera_speed_delta;
        this->camera->set_position(camera_position);
    }
    if (Input::is_action_pressed("move_right")) {
        camera_position.x += camera_speed_delta;
        this->camera->set_position(camera_position);
    }
    if (Input::is_action_pressed("move_forward")) {
        camera_position.y += camera_speed_delta;
        this->camera->set_position(camera_position);
    }
    if (Input::is_action_pressed("move_backward")) {
        camera_position.y -= camera_speed_delta;
        this->camera->set_position(camera_position);
    }

    ctx->renderer->begin_frame(*this->camera);

    static float static_x = 0.0f;
    static float static_y = 0.0f;
    static_x += 0.2f * ctx->delta_time;
    static_y += 0.1f * ctx->delta_time;
    const glm::vec3 position = {static_x, static_y, 0.0f};
    const glm::mat4 transform = translate(glm::mat4(1.0f), position);
    for (const auto& vertex_array : vertex_arrays) {
        ctx->renderer->draw(vertex_array, this->shader, transform);
    }

    ctx->renderer->end_frame();
}
