#include "pch/enginepch.h"
#include "application.h"

#include "gui/imgui_gui.h"
#include "input/input.h"

#include "platforms/opengl/opengl_shader.h"
#include "rendering/renderer.h"

#include "platforms/opengl/opengl_renderer.h"
#include "platforms/opengl/opengl_vertex_array.h"

Application::Application() {
    Logger::init();
    this->running = true;
}

Application::~Application() = default;

void Application::run() {
    this->window = Window::create("DicyEngine", 1920, 1080);
    this->gui = std::make_shared<ImGuiGUI>(window);

    const auto event_dispatcher = EventDispatcher::get();
    std::vector<Layer*> layers = {};
    register_layers(layers, event_dispatcher, window);

    event_dispatcher->register_global_handler<WindowCloseEvent>([this](const WindowCloseEvent&) {
        this->running = false;
    });

    Input::init(event_dispatcher, window);

    const auto renderer = new OpenGLRenderer();

    std::shared_ptr<VertexArray> vertex_array;
    {
        constexpr float vertices[3 * 7] = {
            -0.5f, -0.5f, 0.0f, 0.8f, 0.1f, 0.1f, 1.0f, //
            0.5f,  -0.5f, 0.0f, 0.8f, 0.1f, 0.1f, 1.0f, //
            0.0f,  0.5f,  0.0f, 0.1f, 0.9f, 0.1f, 1.0f, //
        };
        std::shared_ptr<VertexBuffer> vertex_buffer;
        vertex_buffer.reset(renderer->create_vertex_buffer(vertices, sizeof(vertices)));

        vertex_buffer->set_layout({
            {DataType::FLOAT3, "position"},
            {DataType::FLOAT4, "color"},
        });

        constexpr unsigned int indexes[3] = {0, 1, 2};
        std::shared_ptr<IndexBuffer> index_buffer;
        index_buffer.reset(renderer->create_index_buffer(indexes, 3));

        vertex_array.reset(renderer->create_vertex_array(vertex_buffer, index_buffer));
    }
    std::shared_ptr<VertexArray> square_vertex_array;
    {
        constexpr float vertices[4 * 7] = {
            -0.5f, -0.5f, 0.0f, 0.1f, 0.1f, 0.7f, 1.0f, //
            0.5f,  -0.5f, 0.0f, 0.1f, 0.1f, 0.7f, 1.0f, //
            0.5f,  0.5f,  0.0f, 0.1f, 0.1f, 0.7f, 1.0f, //
            -0.5f, 0.5f,  0.0f, 0.1f, 0.1f, 0.7f, 1.0f, //
        };

        std::shared_ptr<VertexBuffer> vertex_buffer;
        vertex_buffer.reset(renderer->create_vertex_buffer(vertices, sizeof(vertices)));

        vertex_buffer->set_layout({
            {DataType::FLOAT3, "position"},
            {DataType::FLOAT4, "color"},
        });

        constexpr unsigned int indexes[6] = {0, 1, 2, 2, 3, 0};
        std::shared_ptr<IndexBuffer> index_buffer;
        index_buffer.reset(renderer->create_index_buffer(indexes, 6));

        square_vertex_array.reset(renderer->create_vertex_array(vertex_buffer, index_buffer));
    }

    auto camera = Camera(-1.0f, 1.0f, -1.0f, 1.0f);

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
    const Shader* shader = new OpenGLShader(vertex_source, fragment_source);


    while (this->running) {
        renderer->clean();

        shader->bind();
        shader->upload_uniform_mat4("u_view_projection", camera.get_view_projection_matrix());

        renderer->draw(square_vertex_array);
        renderer->draw(vertex_array);

        for (const auto& layer : layers) {
            layer->update();
        }

        this->gui->update();
        this->window->update();
    }
}


void register_layers(std::vector<Layer*>& vector, EventDispatcher* event_dispatcher, const std::shared_ptr<Window>& window) {
    int layer_index = 0;
    // vector.push_back(new ImGuiLayer(layer_index++, event_dispatcher, window));
}
