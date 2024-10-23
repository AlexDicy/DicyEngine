#pragma once

class Renderer;

class SkyboxCube {
public:
    SkyboxCube(const Ref<Renderer>& renderer, const Ref<Shader>& shader, const Ref<Texture>& texture);

    const Ref<Shader>& getShader() const {
        return this->shader;
    }

    const Ref<Texture>& getTexture() const {
        return this->texture;
    }

    const Ref<VertexArray>& getVertexArray() const {
        return this->vertexArray;
    }

private:
    Ref<Shader> shader;
    Ref<Texture> texture;
    Ref<VertexArray> vertexArray;
};
