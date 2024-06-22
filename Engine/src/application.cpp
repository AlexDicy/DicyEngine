#include "pch/enginepch.h"
#include "application.h"

#include "gui/imgui_gui.h"
#include "input/input.h"

#include "platforms/opengl/opengl_shader.h"
#include "rendering/renderer.h"

#include "glad/gl.h"
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

    Renderer* renderer = new Renderer();

    const VertexArray* vertex_array;
    {
        float vertices[3 * 7] = {
            -0.5f, -0.5f, 0.0f, 0.8f, 0.1f, 0.1f, 1.0f, //
            0.5f,  -0.5f, 0.0f, 0.8f, 0.1f, 0.1f, 1.0f, //
            0.0f,  0.5f,  0.0f, 0.1f, 0.9f, 0.1f, 1.0f, //
        };
        std::shared_ptr<VertexBuffer> vertex_buffer;
        vertex_buffer.reset(renderer->create_vertex_buffer(vertices, sizeof(vertices)));

        BufferLayout layout = {
            {DataType::FLOAT3, "position"},
            {DataType::FLOAT4, "color"},
        };
        vertex_buffer->set_layout(layout);

        unsigned int indexes[3] = {0, 1, 2};
        std::shared_ptr<IndexBuffer> index_buffer;
        index_buffer.reset(renderer->create_index_buffer(indexes, 3));

        vertex_array = renderer->create_vertex_array(vertex_buffer, index_buffer);
    }

    const std::string vertex_source = R"(
        #version 330 core

        layout(location = 0) in vec3 position;
        layout(location = 1) in vec4 color;

        out vec4 v_color;

        void main() {
            gl_Position = vec4(position, 1.0);
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
    Shader* shader = new OpenGLShader(vertex_source, fragment_source);

    while (this->running) {
        glClearColor(0.1f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader->bind();
        vertex_array->bind();
        glDrawElements(GL_TRIANGLES, vertex_array->get_index_buffer()->get_count(), GL_UNSIGNED_INT,
                       nullptr); // NOLINT(bugprone-narrowing-conversions, cppcoreguidelines-narrowing-conversions)

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
