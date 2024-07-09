#pragma once
#include "camera/camera.h"
#include "vertex_array.h"
#include "shader.h"
#include "texture.h"

enum class RenderAPI { NONE, OPENGL };

class Renderer {
public:
    explicit Renderer(const RenderAPI api) : api(api) {}
    virtual ~Renderer() = default;

    RenderAPI get_api() const {
        return this->api;
    }

    void set_camera(const Ref<Camera>& camera) {
        this->camera = camera;
    }

    virtual void init() const = 0;
    virtual void set_viewport(int x, int y, uint32_t width, uint32_t height) = 0;

    virtual Ref<VertexArray> create_vertex_array(const Ref<VertexBuffer>& vertex_buffer, const Ref<IndexBuffer>& index_buffer) const = 0;
    virtual Ref<VertexBuffer> create_vertex_buffer(const float* vertices, uint32_t size) const = 0;
    virtual Ref<IndexBuffer> create_index_buffer(const uint32_t* indexes, uint32_t count) const = 0;
    virtual Ref<Shader> create_shader(const std::string& vertex_path, const std::string& fragment_path) const = 0;
    virtual Ref<Texture2D> create_texture2d(const std::string& path) const = 0;

    virtual void begin_frame() = 0;
    virtual void end_frame() const = 0;
    virtual void clean() const = 0;
    virtual void draw(const Ref<VertexArray>& vertex_array, const Ref<Shader>& shader, const glm::mat4& transform) const = 0;

protected:
    Ref<Camera> camera;
    glm::mat4 view_projection_matrix;

private:
    RenderAPI api;
};
