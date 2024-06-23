#pragma once
#include "rendering/shader.h"

class OpenGLShader : public Shader {
    uint32_t id;

public:
    OpenGLShader(const std::string& vertex_source, const std::string& fragment_source);
    ~OpenGLShader() override;

    void bind() const override;

    void upload_uniform_mat4(const std::string& name, const glm::mat4& matrix) const override;
};
