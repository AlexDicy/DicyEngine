#include "pch/enginepch.h"
#include "opengl_renderer.h"

#include "opengl_buffer.h"
#include "opengl_framebuffer.h"
#include "opengl_shader.h"
#include "opengl_texture_2d.h"
#include "opengl_texture_cube.h"
#include "opengl_vertex_array.h"

#include <glad/gl.h>
#include <glm/glm.hpp>

void OpenGLRenderer::init(const int x, const int y, const uint32_t width, const uint32_t height) {
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
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

Ref<TextureCube> OpenGLRenderer::create_texture_cube(const std::array<std::string, 6>& paths) const {
    return std::make_shared<OpenGLTextureCube>(paths);
}

Ref<TextureCube> OpenGLRenderer::create_texture_cube_from_hdr(const Ref<Texture2D>& hdr_texture, const Ref<Shader>& convert_shader, const uint32_t size) {
    uint32_t capture_framebuffer;
    uint32_t capture_renderbuffer;
    glGenFramebuffers(1, &capture_framebuffer);
    glGenRenderbuffers(1, &capture_renderbuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, capture_framebuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, capture_renderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, size, size);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, capture_renderbuffer);

    convert_shader->bind();
    hdr_texture->bind(0);
    convert_shader->upload_uniform_int("u_equirectangular_map", 0);

    uint32_t cube_map_id;
    glGenTextures(1, &cube_map_id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cube_map_id);
    for (unsigned int i = 0; i < 6; i++) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, size, size, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    const auto projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    glm::mat4 view_matrices[] = {
        lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)), // right
        lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)), // left
        lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)), // top
        lookAt(glm::vec3(0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f)), // bottom
        lookAt(glm::vec3(0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)), // front
        lookAt(glm::vec3(0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)), // back
    };
    int previous_viewport[4];
    glGetIntegerv(GL_VIEWPORT, previous_viewport);
    glViewport(0, 0, size, size);
    const SkyboxCube cube(this->shared_from_this(), convert_shader, hdr_texture);
    for (unsigned int i = 0; i < 6; i++) {
        convert_shader->upload_uniform_mat4("u_view_projection", projection * view_matrices[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cube_map_id, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        cube.get_vertex_array()->bind();
        glDrawElements(GL_TRIANGLES, static_cast<int>(cube.get_vertex_array()->get_index_buffer()->get_count()), GL_UNSIGNED_INT, nullptr);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(previous_viewport[0], previous_viewport[1], previous_viewport[2], previous_viewport[3]);
    glDeleteFramebuffers(1, &capture_framebuffer);
    glDeleteRenderbuffers(1, &capture_renderbuffer);
    return std::make_shared<OpenGLTextureCube>(cube_map_id);
}


void OpenGLRenderer::begin_frame() {
    this->view_projection_matrix = this->camera->get_view_projection_matrix(true);
    this->view_matrix = this->camera->get_view_matrix();
    this->projection_matrix = this->camera->get_projection_matrix();
    this->point_lights.clear();
    this->framebuffer->bind();
    this->clean(); // make sure to clean the framebuffer
    glEnable(GL_CULL_FACE); // disabled by the skybox
    glDepthFunc(GL_LESS); // changed by the skybox
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

void OpenGLRenderer::draw_skybox(const Ref<SkyboxCube>& skybox) const {
    skybox->get_shader()->bind();
    // remove the translation part of the view matrix
    const auto view_matrix = glm::mat4(glm::mat3(this->view_matrix));
    const auto view_projection_matrix = this->projection_matrix * view_matrix;
    skybox->get_shader()->upload_uniform_mat4("u_view_projection", view_projection_matrix);
    skybox->get_texture()->bind(0);
    skybox->get_shader()->upload_uniform_int("u_skybox", 0);
    skybox->get_vertex_array()->bind();
    glDepthFunc(GL_LEQUAL);
    glDisable(GL_CULL_FACE);
    glDrawElements(GL_TRIANGLES, static_cast<int>(skybox->get_vertex_array()->get_index_buffer()->get_count()), GL_UNSIGNED_INT, nullptr);
}
