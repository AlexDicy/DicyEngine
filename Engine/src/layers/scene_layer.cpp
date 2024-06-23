#include "pch/enginepch.h"
#include "scene_layer.h"

#include "events/event.h"
#include "platforms/opengl/opengl_shader.h"

SceneLayer::SceneLayer(const Application* application) {
    const std::shared_ptr<Renderer>& renderer = application->get_renderer();

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

    application->get_event_dispatcher()->register_global_handler<KeyPressedEvent>([this](const KeyPressedEvent& event) {
        const auto current_position = this->camera->get_position();
        if (event.get_key() == InputCode::KEY_LEFT) {
            this->camera->set_position({current_position.x - 0.02f, current_position.y, current_position.z});
        } else if (event.get_key() == InputCode::KEY_RIGHT) {
            this->camera->set_position({current_position.x + 0.02f, current_position.y, current_position.z});
        } else if (event.get_key() == InputCode::KEY_UP) {
            this->camera->set_position({current_position.x, current_position.y + 0.02f, current_position.z});
        } else if (event.get_key() == InputCode::KEY_DOWN) {
            this->camera->set_position({current_position.x, current_position.y - 0.02f, current_position.z});
        }
    });

    const std::string vertex_source = R"(
        #version 330 core

        layout(location = 0) in vec3 position;
        layout(location = 1) in vec4 color;

        uniform mat4 u_view_projection;

        out vec4 v_color;

        void main() {
            gl_Position = u_view_projection * vec4(position, 1.0);
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

void SceneLayer::update(const std::shared_ptr<Renderer>& renderer) {
    renderer->begin_frame(*this->camera);

    for (const auto& vertex_array : vertex_arrays) {
        renderer->draw(vertex_array, this->shader);
    }

    renderer->end_frame();
}
