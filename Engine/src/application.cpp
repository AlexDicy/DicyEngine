#include "pch/enginepch.h"
#include "glad/gl.h"

#include "application.h"

#include "gui/imgui_gui.h"
#include "input/input.h"
#include "platforms/opengl/opengl_datatype.h"

#include "platforms/opengl/opengl_shader.h"
#include "rendering/renderer.h"


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
    float vertices[3 * 7] = {
        -0.5f, -0.5f, 0.0f, 0.8f, 0.1f, 0.1f, 1.0f, //
        0.5f,  -0.5f, 0.0f, 0.8f, 0.1f, 0.1f, 1.0f, //
        0.0f,  0.5f,  0.0f, 0.1f, 0.9f, 0.1f, 1.0f, //
    };
    this->vertex_buffer.reset(renderer->create_vertex_buffer(vertices, sizeof(vertices)));

    BufferLayout layout = {
        {DataType::FLOAT3, "position"},
        {DataType::FLOAT4, "color"},
    };

    int layout_index = 0;
    for (auto& attribute : layout.get_attributes()) {
        glEnableVertexAttribArray(layout_index);
        glVertexAttribPointer(
            layout_index, //
            attribute.get_datatype_count(), // 
            get_opengl_type_from_datatype(attribute.type), //
            attribute.is_normalized ? GL_TRUE : GL_FALSE, //
            layout.get_size(),  // NOLINT(bugprone-narrowing-conversions, cppcoreguidelines-narrowing-conversions)
            reinterpret_cast<const void*>(attribute.offset)  // NOLINT(performance-no-int-to-ptr)
            );
        layout_index++;
    }

    unsigned int indexes[3] = {0, 1, 2};
    this->index_buffer.reset(renderer->create_index_buffer(indexes, 3));

    const std::string vertex_source = R"(
        #version 330 core

        layout(location = 0) in vec3 position;
        layout(location = 0) in vec4 color;

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
    shader->bind();

    while (this->running) {
        glClearColor(0.1f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

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
