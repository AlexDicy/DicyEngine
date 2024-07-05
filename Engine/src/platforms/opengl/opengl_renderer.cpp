#include "pch/enginepch.h"
#include "opengl_renderer.h"

#include "opengl_buffer.h"
#include "opengl_vertex_array.h"

#include <glad/gl.h>
#include <glm/glm.hpp>

void OpenGLRenderer::set_viewport(const int x, const int y, const uint32_t width, const uint32_t height) {
    glViewport(x, y, width, height);  // NOLINT(bugprone-narrowing-conversions, cppcoreguidelines-narrowing-conversions)
}

VertexArray* OpenGLRenderer::create_vertex_array(const std::shared_ptr<VertexBuffer>& vertex_buffer, const std::shared_ptr<IndexBuffer>& index_buffer) const {
    return new OpenGLVertexArray(vertex_buffer, index_buffer);
}

VertexBuffer* OpenGLRenderer::create_vertex_buffer(const float* vertices, const uint32_t size) const {
    return new OpenGLVertexBuffer(vertices, size);
}

IndexBuffer* OpenGLRenderer::create_index_buffer(const uint32_t* indexes, const uint32_t count) const {
    return new OpenGLIndexBuffer(indexes, count);
}
void OpenGLRenderer::begin_frame(Camera& camera) {
    this->view_projection_matrix = camera.get_view_projection_matrix(true);
}

void OpenGLRenderer::end_frame() const {}

void OpenGLRenderer::clean() const {
    glClearColor(0.1f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRenderer::draw(const std::shared_ptr<VertexArray>& vertex_array, const std::shared_ptr<Shader>& shader, const glm::mat4& transform) const {
    shader->bind();
    shader->upload_uniform_mat4("u_view_projection", this->view_projection_matrix);
    shader->upload_uniform_mat4("u_transform", transform);

    vertex_array->bind();
    glDrawElements(GL_TRIANGLES, vertex_array->get_index_buffer()->get_count(), // NOLINT(bugprone-narrowing-conversions, cppcoreguidelines-narrowing-conversions)
                   GL_UNSIGNED_INT, nullptr);
}
