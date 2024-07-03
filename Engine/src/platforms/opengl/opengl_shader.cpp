#include "pch/enginepch.h"
#include "opengl_shader.h"

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

OpenGLShader::OpenGLShader(const std::string& vertex_source, const std::string& fragment_source) {
    // Create an empty vertex shader handle
    const GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);

    // Send the vertex shader source code to GL
    // Note that std::string's .c_str is NULL character terminated.
    const GLchar* source = vertex_source.c_str();
    glShaderSource(vertex_shader, 1, &source, 0);

    // Compile the vertex shader
    glCompileShader(vertex_shader);

    GLint is_compiled = 0;
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &is_compiled);
    if (is_compiled == GL_FALSE) {
        GLint max_length = 0;
        glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &max_length);

        // The maxLength includes the NULL character
        std::vector<GLchar> info_log(max_length);
        glGetShaderInfoLog(vertex_shader, max_length, &max_length, info_log.data());

        // We don't need the shader anymore.
        glDeleteShader(vertex_shader);

        DE_ERROR("OpenGL Vertex Shader compilation error: {0}", info_log.data());
        return;
    }

    // Create an empty fragment shader handle
    const GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

    // Send the fragment shader source code to GL
    // Note that std::string's .c_str is NULL character terminated.
    source = fragment_source.c_str();
    glShaderSource(fragment_shader, 1, &source, 0);

    // Compile the fragment shader
    glCompileShader(fragment_shader);

    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &is_compiled);
    if (is_compiled == GL_FALSE) {
        GLint max_length = 0;
        glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &max_length);

        // The maxLength includes the NULL character
        std::vector<GLchar> info_log(max_length);
        glGetShaderInfoLog(fragment_shader, max_length, &max_length, info_log.data());

        // We don't need the shader anymore.
        glDeleteShader(fragment_shader);
        // Either of them. Don't leak shaders.
        glDeleteShader(vertex_shader);

        DE_ERROR("OpenGL Fragment Shader compilation error: {0}", info_log.data());
        return;
    }

    // Vertex and fragment shaders are successfully compiled.
    // Now time to link them together into a program.
    // Get a program object.
    this->id = glCreateProgram();

    // Attach our shaders to our program
    glAttachShader(this->id, vertex_shader);
    glAttachShader(this->id, fragment_shader);

    // Link our program
    glLinkProgram(this->id);

    // Note the different functions here: glGetProgram* instead of glGetShader*.
    GLint is_linked = 0;
    glGetProgramiv(this->id, GL_LINK_STATUS, &is_linked);
    if (is_linked == GL_FALSE) {
        GLint max_length = 0;
        glGetProgramiv(this->id, GL_INFO_LOG_LENGTH, &max_length);

        // The maxLength includes the NULL character
        std::vector<GLchar> info_log(max_length);
        glGetProgramInfoLog(this->id, max_length, &max_length, info_log.data());

        // We don't need the program anymore.
        glDeleteProgram(this->id);
        // Don't leak shaders either.
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);

        DE_ERROR("OpenGL Shader linking error: {0}", info_log.data());
        return;
    }

    // Always detach shaders after a successful link.
    glDetachShader(this->id, vertex_shader);
    glDetachShader(this->id, fragment_shader);
}

OpenGLShader::~OpenGLShader() {
    glDeleteProgram(this->id);
}
void OpenGLShader::bind() const {
    glUseProgram(this->id);
}

void OpenGLShader::upload_uniform_vec4(const std::string& name, const glm::vec4& vector) const {
    const GLint location = glGetUniformLocation(this->id, name.c_str());
    glProgramUniform4fv(this->id, location, 1, value_ptr(vector));
}

void OpenGLShader::upload_uniform_mat4(const std::string& name, const glm::mat4& matrix) const {
    const GLint location = glGetUniformLocation(this->id, name.c_str());
    glProgramUniformMatrix4fv(this->id, location, 1, GL_FALSE, value_ptr(matrix));
}
