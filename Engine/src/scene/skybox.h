#pragma once

class Renderer;

class Skybox {
public:
    Skybox(const Ref<Renderer>& renderer, const Ref<Shader>& shader, const Ref<TextureCube>& texture);

    const Ref<Shader>& get_shader() const {
        return this->shader;
    }

    const Ref<TextureCube>& get_texture() const {
        return this->texture;
    }

    const Ref<VertexArray>& get_vertex_array() const {
        return this->vertex_array;
    }

private:
    Ref<Shader> shader;
    Ref<TextureCube> texture;
    Ref<VertexArray> vertex_array;
};
