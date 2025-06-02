#pragma once
#include "rendering/Shader.h"

class OpenGLShader : public Shader {
    uint32_t id;

public:
    OpenGLShader(const std::string& vertexPath, const std::string& fragmentPath);
    ~OpenGLShader() override;

    void bind() const override;

    void uploadUniformInt(const std::string& name, int value) const override;
    void uploadUniformFloat(const std::string& name, float value) const override;
    void uploadUniformVec2Int(const std::string& name, const glm::ivec2& vector) const override;
    void uploadUniformVec3(const std::string& name, const glm::vec3& vector) const override;
    void uploadUniformVec4(const std::string& name, const glm::vec4& vector) const override;
    void uploadUniformMat3(const std::string& name, const glm::mat3& matrix) const override;
    void uploadUniformMat4(const std::string& name, const glm::mat4& matrix) const override;
};
