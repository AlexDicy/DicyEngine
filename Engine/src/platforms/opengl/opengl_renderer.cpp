#include "pch/enginepch.h"
#include "opengl_renderer.h"

#include "opengl_buffer.h"
#include "opengl_vertex_array.h"

#include <glad/gl.h>

VertexArray* OpenGLRenderer::create_vertex_array(const std::shared_ptr<VertexBuffer>& vertex_buffer, const std::shared_ptr<IndexBuffer>& index_buffer) const {
    return new OpenGLVertexArray(vertex_buffer, index_buffer);
}

VertexBuffer* OpenGLRenderer::create_vertex_buffer(const float* vertices, const uint32_t size) const {
    return new OpenGLVertexBuffer(vertices, size);
}

IndexBuffer* OpenGLRenderer::create_index_buffer(const uint32_t* indexes, const uint32_t count) const {
    return new OpenGLIndexBuffer(indexes, count);
}

void OpenGLRenderer::clean() const {
    glClearColor(0.1f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLRenderer::draw(const std::shared_ptr<VertexArray>& vertex_array) const {
    vertex_array->bind();
    glDrawElements(GL_TRIANGLES, vertex_array->get_index_buffer()->get_count(), // NOLINT(bugprone-narrowing-conversions, cppcoreguidelines-narrowing-conversions)
                   GL_UNSIGNED_INT, nullptr);
}
