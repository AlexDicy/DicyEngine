#include "pch/enginepch.h"
#include "OpenGLShader.h"

#include <fstream>
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// strips the BOM character which is not supported by macOS OpenGL implementation
inline std::string streamToString(std::ifstream& input) {
    std::stringstream stringStream;
    char bom[3];
    input.read(bom, 3);
    if (bom[0] == '\xEF' && bom[1] == '\xBB' && bom[2] == '\xBF') {
        stringStream << input.rdbuf();
    } else {
        input.seekg(0, std::ios::beg);
        stringStream << input.rdbuf();
    }
    return stringStream.str();
}

OpenGLShader::OpenGLShader(const std::string& vertexPath, const std::string& fragmentPath) {
    auto vertexIn = std::ifstream(vertexPath, std::ios::in | std::ios::binary);
    auto fragmentIn = std::ifstream(fragmentPath, std::ios::in | std::ios::binary);

    if (!vertexIn) {
        DE_ERROR("Cannot open vertex shader file {0}", vertexPath);
        return;
    }
    if (!fragmentIn) {
        DE_ERROR("Cannot open fragment shader file {0}", fragmentPath);
        return;
    }

    const std::string vertexSource = streamToString(vertexIn);
    const std::string fragmentSource = streamToString(fragmentIn);

    // Create an empty vertex shader handle
    const GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // Send the vertex shader source code to GL
    // Note that std::string's .c_str is NULL character terminated.
    const GLchar* source = vertexSource.c_str();
    glShaderSource(vertexShader, 1, &source, 0);

    // Compile the vertex shader
    glCompileShader(vertexShader);

    GLint isCompiled = 0;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE) {
        GLint maxLength = 0;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(vertexShader, maxLength, &maxLength, infoLog.data());

        // We don't need the shader anymore.
        glDeleteShader(vertexShader);

        DE_ERROR("OpenGL Vertex Shader compilation error: {0}", infoLog.data());
        return;
    }

    // Create an empty fragment shader handle
    const GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Send the fragment shader source code to GL
    // Note that std::string's .c_str is NULL character terminated.
    source = fragmentSource.c_str();
    glShaderSource(fragmentShader, 1, &source, 0);

    // Compile the fragment shader
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE) {
        GLint maxLength = 0;
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, infoLog.data());

        // We don't need the shader anymore.
        glDeleteShader(fragmentShader);
        // Either of them. Don't leak shaders.
        glDeleteShader(vertexShader);

        DE_ERROR("OpenGL Fragment Shader compilation error: {0}", infoLog.data());
        return;
    }

    // Vertex and fragment shaders are successfully compiled.
    // Now time to link them together into a program.
    // Get a program object.
    this->id = glCreateProgram();

    // Attach our shaders to our program
    glAttachShader(this->id, vertexShader);
    glAttachShader(this->id, fragmentShader);

    // Link our program
    glLinkProgram(this->id);

    // Note the different functions here: glGetProgram* instead of glGetShader*.
    GLint isLinked = 0;
    glGetProgramiv(this->id, GL_LINK_STATUS, &isLinked);
    if (isLinked == GL_FALSE) {
        GLint maxLength = 0;
        glGetProgramiv(this->id, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(this->id, maxLength, &maxLength, infoLog.data());

        // We don't need the program anymore.
        glDeleteProgram(this->id);
        // Don't leak shaders either.
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        DE_ERROR("OpenGL Shader linking error: {0}", infoLog.data());
        return;
    }

    // Always detach shaders after a successful link.
    glDetachShader(this->id, vertexShader);
    glDetachShader(this->id, fragmentShader);
}

OpenGLShader::~OpenGLShader() {
    glDeleteProgram(this->id);
}
void OpenGLShader::bind() const {
    glUseProgram(this->id);
}


void OpenGLShader::uploadUniformInt(const std::string& name, const int value) const {
    const GLint location = glGetUniformLocation(this->id, name.c_str());
    DE_ASSERT(location >= 0, "Uniform {0} not found", name)
    glProgramUniform1i(this->id, location, value);
}

void OpenGLShader::uploadUniformFloat(const std::string& name, const float value) const {
    const GLint location = glGetUniformLocation(this->id, name.c_str());
    DE_ASSERT(location >= 0, "Uniform {0} not found", name)
    glProgramUniform1f(this->id, location, value);
}

void OpenGLShader::uploadUniformVec2Int(const std::string& name, const glm::ivec2& vector) const {
    const GLint location = glGetUniformLocation(this->id, name.c_str());
    DE_ASSERT(location >= 0, "Uniform {0} not found", name)
    glProgramUniform2iv(this->id, location, 1, glm::value_ptr(vector));
}

void OpenGLShader::uploadUniformVec2(const std::string& name, const glm::vec2& vector) const {
    const GLint location = glGetUniformLocation(this->id, name.c_str());
    DE_ASSERT(location >= 0, "Uniform {0} not found", name)
    glProgramUniform2fv(this->id, location, 1, glm::value_ptr(vector));
}

void OpenGLShader::uploadUniformVec3(const std::string& name, const glm::vec3& vector) const {
    const GLint location = glGetUniformLocation(this->id, name.c_str());
    DE_ASSERT(location >= 0, "Uniform {0} not found", name)
    glProgramUniform3fv(this->id, location, 1, value_ptr(vector));
}

void OpenGLShader::uploadUniformVec4(const std::string& name, const glm::vec4& vector) const {
    const GLint location = glGetUniformLocation(this->id, name.c_str());
    DE_ASSERT(location >= 0, "Uniform {0} not found", name)
    glProgramUniform4fv(this->id, location, 1, value_ptr(vector));
}

void OpenGLShader::uploadUniformMat3(const std::string& name, const glm::mat3& matrix) const {
    const GLint location = glGetUniformLocation(this->id, name.c_str());
    DE_ASSERT(location >= 0, "Uniform {0} not found", name)
    glProgramUniformMatrix3fv(this->id, location, 1, GL_FALSE, value_ptr(matrix));
}

void OpenGLShader::uploadUniformMat4(const std::string& name, const glm::mat4& matrix) const {
    const GLint location = glGetUniformLocation(this->id, name.c_str());
    DE_ASSERT(location >= 0, "Uniform {0} not found", name)
    glProgramUniformMatrix4fv(this->id, location, 1, GL_FALSE, value_ptr(matrix));
}
