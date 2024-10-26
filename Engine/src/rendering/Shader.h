#pragma once
#include <glm/glm.hpp>

class Shader {
public:
    virtual ~Shader() = default;

    virtual void bind() const = 0;

    virtual void uploadUniformInt(const std::string& name, int value) const = 0;
    virtual void uploadUniformFloat(const std::string& name, float value) const = 0;
    virtual void uploadUniformVec3(const std::string& name, const glm::vec3& vector) const = 0;
    virtual void uploadUniformVec4(const std::string& name, const glm::vec4& vector) const = 0;
    virtual void uploadUniformMat3(const std::string& name, const glm::mat3& matrix) const = 0;
    virtual void uploadUniformMat4(const std::string& name, const glm::mat4& matrix) const = 0;
};
