#include "pch/enginepch.h"
#include "opengl_texture_cube.h"

#include <glad/gl.h>
#include <stb_image.h>


OpenGLTextureCube::OpenGLTextureCube(const std::array<std::string, 6>& paths) : paths(paths) {
    glGenTextures(1, &this->id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->id);

    int width = -1;
    int height = -1;
    int channels = 0;
    for (unsigned int i = 0; i < paths.size(); i++) {
        stbi_uc* texture = stbi_load(paths[i].c_str(), &width, &height, &channels, 0);
        if (!texture) {
            const char* error = stbi_failure_reason();
            DE_ERROR("Failed to read texture file {0} - {1}", paths[i], error);
        }

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);
        stbi_image_free(texture);
    }

    this->size = width;

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

OpenGLTextureCube::OpenGLTextureCube(const uint32_t id, const uint32_t size) : id(id), size(size) {}

OpenGLTextureCube::~OpenGLTextureCube() {
    glDeleteTextures(1, &this->id);
}

void OpenGLTextureCube::bind(uint32_t slot) const {
#ifdef OPENGL_4_6
    glBindTextureUnit(slot, this->id);
#else
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->id);
#endif
}

void setup_render_buffer(uint32_t& capture_framebuffer, uint32_t& capture_renderbuffer, const uint32_t size) {
    glGenFramebuffers(1, &capture_framebuffer);
    glGenRenderbuffers(1, &capture_renderbuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, capture_framebuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, capture_renderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, size, size);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, capture_renderbuffer);
}

void create_cube_map(uint32_t& cube_map_id, const uint32_t size) {
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
}

void render_to_cubemap(const Ref<Renderer>& renderer, const Ref<Shader>& convert_shader, const Ref<Texture>& texture, const uint32_t cube_map_id) {
    const auto projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    const glm::mat4 view_matrices[] = {
        lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)), // right
        lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)), // left
        lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)), // top
        lookAt(glm::vec3(0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f)), // bottom
        lookAt(glm::vec3(0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)), // front
        lookAt(glm::vec3(0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)), // back
    };
    const SkyboxCube cube(renderer, convert_shader, texture);
    for (unsigned int i = 0; i < 6; i++) {
        convert_shader->upload_uniform_mat4("u_view_projection", projection * view_matrices[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cube_map_id, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        cube.get_vertex_array()->bind();
        glDrawElements(GL_TRIANGLES, static_cast<int>(cube.get_vertex_array()->get_index_buffer()->get_count()), GL_UNSIGNED_INT, nullptr);
    }
}

Ref<TextureCube> OpenGLTextureCube::create_from_hdr(const Ref<Renderer>& renderer, const Ref<Texture2D>& hdr_texture, const Ref<Shader>& convert_shader, const uint32_t size) {
    uint32_t capture_framebuffer;
    uint32_t capture_renderbuffer;
    setup_render_buffer(capture_framebuffer, capture_renderbuffer, size);

    uint32_t cube_map_id;
    create_cube_map(cube_map_id, size);

    convert_shader->bind();
    hdr_texture->bind(0);
    convert_shader->upload_uniform_int("u_equirectangular_map", 0);

    int previous_viewport[4];
    glGetIntegerv(GL_VIEWPORT, previous_viewport);
    glViewport(0, 0, size, size);
    render_to_cubemap(renderer, convert_shader, hdr_texture, cube_map_id);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(previous_viewport[0], previous_viewport[1], previous_viewport[2], previous_viewport[3]);
    glDeleteFramebuffers(1, &capture_framebuffer);
    glDeleteRenderbuffers(1, &capture_renderbuffer);
    return std::make_shared<OpenGLTextureCube>(cube_map_id, size);
}

Ref<TextureCube> OpenGLTextureCube::create_irradiance_map(const Ref<Renderer>& renderer, const Ref<TextureCube>& texture_cube, const Ref<Shader>& irradiance_shader,
                                                          const uint32_t size) {
    uint32_t capture_framebuffer;
    uint32_t capture_renderbuffer;
    setup_render_buffer(capture_framebuffer, capture_renderbuffer, size);

    uint32_t irradiance_map_id;
    create_cube_map(irradiance_map_id, size);

    irradiance_shader->bind();
    texture_cube->bind(0);
    irradiance_shader->upload_uniform_int("u_environment_map", 0);

    int previous_viewport[4];
    glGetIntegerv(GL_VIEWPORT, previous_viewport);
    glViewport(0, 0, size, size);
    render_to_cubemap(renderer, irradiance_shader, texture_cube, irradiance_map_id);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(previous_viewport[0], previous_viewport[1], previous_viewport[2], previous_viewport[3]);
    glDeleteFramebuffers(1, &capture_framebuffer);
    glDeleteRenderbuffers(1, &capture_renderbuffer);
    return std::make_shared<OpenGLTextureCube>(irradiance_map_id, size);
}
