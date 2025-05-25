#pragma once
#include "rendering/Shader.h"
#include "rendering/Renderer.h"

class ShaderRegistry {
public:
    explicit ShaderRegistry(const Ref<Renderer>& renderer) : renderer(renderer) {}

    // name is usually the file path without the extension
    Ref<Shader> load(const std::string& name) {
        Ref<Shader> shader = renderer->createShader(name + ".dshv", name + ".dshf");
        this->add(name, shader);
        return shader;
    }

    void add(const std::string& name, const Ref<Shader>& shader) {
        this->shaders[name] = shader;
    }

    Ref<Shader> get(const std::string& name) {
        return this->shaders.at(name);
    }

private:
    Ref<Renderer> renderer;
    std::unordered_map<std::string, Ref<Shader>> shaders;
};
