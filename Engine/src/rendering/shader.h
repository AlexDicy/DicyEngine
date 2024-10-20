﻿#pragma once
#include <glm/glm.hpp>

class Shader {
public:
    virtual ~Shader() = default;

    virtual void bind() const = 0;

    virtual void upload_uniform_int(const std::string& name, int value) const = 0;
    virtual void upload_uniform_float(const std::string& name, float value) const = 0;
    virtual void upload_uniform_vec3(const std::string& name, const glm::vec3& vector) const = 0;
    virtual void upload_uniform_vec4(const std::string& name, const glm::vec4& vector) const = 0;
    virtual void upload_uniform_mat4(const std::string& name, const glm::mat4& matrix) const = 0;
};
