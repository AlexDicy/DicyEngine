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
    unsigned char white[4] = {255, 255, 255, 255};
    this->white_pixel_texture = std::make_shared<OpenGLTexture2D>(1, 1, 1, white);
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

Ref<Texture2D> OpenGLRenderer::create_texture2d(const unsigned int channels, const unsigned int width, const unsigned int height, const void* data) const {
    return std::make_shared<OpenGLTexture2D>(channels, width, height, data);
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
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRenderer::draw(const Ref<VertexArray>& vertex_array, const Ref<Shader>& shader, const glm::mat4& transform, const Ref<DirectionalLight>& directional_light) const {
    this->draw(vertex_array, shader, transform, directional_light, this->white_pixel_texture);
}

void OpenGLRenderer::draw(const Ref<VertexArray>& vertex_array, const Ref<Shader>& shader, const glm::mat4& transform, const Ref<DirectionalLight>& directional_light,
                          const Ref<Texture>& texture) const {
    texture->bind(0);
    shader->bind();
    shader->upload_uniform_mat4("u_view_projection", this->view_projection_matrix);
    shader->upload_uniform_mat4("u_transform", transform);
    // texture
    shader->upload_uniform_int("u_texture", 0);
    // lights
    shader->upload_uniform_vec3("u_material.ambient_color", {1.0f, 1.0f, 1.0f});
    shader->upload_uniform_vec3("u_ambient_light.color", {1.0f, 1.0f, 1.0f});
    shader->upload_uniform_float("u_ambient_light.intensity", 0.12f);
    shader->upload_uniform_vec3("u_directional_light.color", {1.0f, 1.0f, 1.0f});
    shader->upload_uniform_float("u_directional_light.intensity", directional_light->get_intensity());
    shader->upload_uniform_vec3("u_directional_light.direction", directional_light->get_local_direction(transform));
    const auto camera_position_local = glm::vec3(inverse(transform) * glm::vec4(this->camera->get_position(), 1.0f));
    shader->upload_uniform_vec3("u_camera_position_local", camera_position_local);

    vertex_array->bind();
    glDrawElements(GL_TRIANGLES, static_cast<int>(vertex_array->get_index_buffer()->get_count()), GL_UNSIGNED_INT, nullptr);
}
