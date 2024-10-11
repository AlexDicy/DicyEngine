#pragma once
#include "rendering/shader.h"

class OpenGLShader : public Shader {
    uint32_t id;

public:
    OpenGLShader(const std::string& vertex_path, const std::string& fragment_path);
    ~OpenGLShader() override;

    void bind() const override;

    void upload_uniform_int(const std::string& name, int value) const override;
    void upload_uniform_float(const std::string& name, float value) const override;
    void upload_uniform_vec3(const std::string& name, const glm::vec3& vector) const override;
    void upload_uniform_vec4(const std::string& name, const glm::vec4& vector) const override;
    void upload_uniform_mat4(const std::string& name, const glm::mat4& matrix) const override;
};
