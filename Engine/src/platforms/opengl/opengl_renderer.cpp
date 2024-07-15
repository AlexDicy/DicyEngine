#include "pch/enginepch.h"
#include "opengl_renderer.h"

#include "opengl_buffer.h"
#include "opengl_framebuffer.h"
#include "opengl_shader.h"
#include "opengl_texture.h"
#include "opengl_vertex_array.h"

#include <glad/gl.h>
#include <glm/glm.hpp>

void OpenGLRenderer::init(const int x, const int y, const uint32_t width, const uint32_t height) {
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    this->set_viewport(x, y, width, height);
}

void OpenGLRenderer::set_viewport(const int x, const int y, const uint32_t width, const uint32_t height) {
    glViewport(x, y, static_cast<int>(width), static_cast<int>(height));
    if (this->camera) {
        this->camera->set_aspect_ratio(static_cast<float>(width) / static_cast<float>(height));
    }
    this->framebuffer = std::make_shared<OpenGLFramebuffer>(width, height);
}

const Ref<Framebuffer>& OpenGLRenderer::get_framebuffer() const {
    return this->framebuffer;
}


Ref<VertexArray> OpenGLRenderer::create_vertex_array(const Ref<VertexBuffer>& vertex_buffer, const Ref<IndexBuffer>& index_buffer) const {
    return std::make_shared<OpenGLVertexArray>(vertex_buffer, index_buffer);
}

Ref<VertexBuffer> OpenGLRenderer::create_vertex_buffer(const float* vertices, const uint32_t size) const {
    return std::make_shared<OpenGLVertexBuffer>(vertices, size);
}

Ref<IndexBuffer> OpenGLRenderer::create_index_buffer(const uint32_t* indexes, const uint32_t count) const {
    return std::make_shared<OpenGLIndexBuffer>(indexes, count);
}

Ref<Shader> OpenGLRenderer::create_shader(const std::string& vertex_path, const std::string& fragment_path) const {
    return std::make_shared<OpenGLShader>(vertex_path, fragment_path);
}

Ref<Texture2D> OpenGLRenderer::create_texture2d(const std::string& path) const {
    return std::make_shared<OpenGLTexture2D>(path);
}


void OpenGLRenderer::begin_frame() {
    this->view_projection_matrix = this->camera->get_view_projection_matrix(true);
    this->framebuffer->bind();
    this->clean(); // make sure to clean the framebuffer
}

void OpenGLRenderer::end_frame() const {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLRenderer::clean() const {
    DE_PROFILE_FUNCTION();
    glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRenderer::draw(const Ref<VertexArray>& vertex_array, const Ref<Shader>& shader, const glm::mat4& transform) const {
    shader->bind();
    shader->upload_uniform_mat4("u_view_projection", this->view_projection_matrix);
    shader->upload_uniform_mat4("u_transform", transform);

    vertex_array->bind();
    glDrawElements(GL_TRIANGLES, static_cast<int>(vertex_array->get_index_buffer()->get_count()), GL_UNSIGNED_INT, nullptr);
}
