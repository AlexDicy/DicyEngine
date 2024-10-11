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
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    this->set_viewport(x, y, width, height);
    unsigned char white[4] = {255, 255, 255, 255};
    this->white_pixel_texture = std::make_shared<OpenGLTexture2D>(1, 1, 1, white);
    this->default_occlusion_roughness_metallic_texture = std::make_shared<OpenGLTexture2D>(3, 1, 1, std::array<unsigned char, 3>{255, 255, 0}.data());
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
    this->point_lights.clear();
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

void OpenGLRenderer::add_point_light(const PointLight& point_light) {
    this->point_lights.push_back(point_light);
}

void OpenGLRenderer::draw(const Ref<VertexArray>& vertex_array, const Ref<Shader>& shader, const glm::mat4& transform, const Ref<DirectionalLight>& directional_light) const {
    this->draw(vertex_array, shader, transform, directional_light, Material(this->white_pixel_texture));
}

void OpenGLRenderer::draw(const Ref<VertexArray>& vertex_array, const Ref<Shader>& shader, const glm::mat4& transform, const Ref<DirectionalLight>& directional_light,
                          const Material& material) const {
    shader->bind();
    shader->upload_uniform_mat4("u_view_projection", this->view_projection_matrix);
    shader->upload_uniform_mat4("u_transform", transform);
    // texture
    int texture_slot = 0;
    material.albedo->bind(texture_slot);
    shader->upload_uniform_int("u_albedo", texture_slot++);
    if (material.occlusion_roughness_metallic) {
        material.occlusion_roughness_metallic->bind(texture_slot);
    } else {
        this->default_occlusion_roughness_metallic_texture->bind(texture_slot);
    }
    shader->upload_uniform_int("u_occlusion_roughness_metallic", texture_slot);
    // lights
    shader->upload_uniform_int("u_material.ignore_lighting", material.ignore_lighting);
    shader->upload_uniform_vec3("u_directional_light.color", {1.0f, 1.0f, 1.0f});
    shader->upload_uniform_float("u_directional_light.intensity", directional_light->get_intensity());
    shader->upload_uniform_vec3("u_directional_light.direction", directional_light->get_local_direction(transform));
    for (size_t i = 0; i < this->point_lights.size(); i++) {
        const PointLight& point_light = this->point_lights[i];
        shader->upload_uniform_vec3("u_point_lights[" + std::to_string(i) + "].position", glm::vec3(inverse(transform) * glm::vec4(point_light.position, 1.0f)));
        shader->upload_uniform_vec3("u_point_lights[" + std::to_string(i) + "].color", point_light.color);
        shader->upload_uniform_float("u_point_lights[" + std::to_string(i) + "].intensity", point_light.intensity);
    }
    shader->upload_uniform_int("u_point_lights_count", static_cast<int>(this->point_lights.size()));
    // camera position
    const auto camera_position_local = glm::vec3(inverse(transform) * glm::vec4(this->camera->get_position(), 1.0f));
    shader->upload_uniform_vec3("u_camera_position_local", camera_position_local);

    vertex_array->bind();
    glDrawElements(GL_TRIANGLES, static_cast<int>(vertex_array->get_index_buffer()->get_count()), GL_UNSIGNED_INT, nullptr);
}
