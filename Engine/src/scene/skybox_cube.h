#pragma once

class Renderer;

class SkyboxCube {
public:
    SkyboxCube(const Ref<Renderer>& renderer, const Ref<Shader>& shader, const Ref<Texture>& texture);

    const Ref<Shader>& get_shader() const {
        return this->shader;
    }

    const Ref<Texture>& get_texture() const {
        return this->texture;
    }

    const Ref<VertexArray>& get_vertex_array() const {
        return this->vertex_array;
    }

private:
    Ref<Shader> shader;
    Ref<Texture> texture;
    Ref<VertexArray> vertex_array;
};
