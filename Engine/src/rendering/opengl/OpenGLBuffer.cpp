#include "pch/enginepch.h"
#include "OpenGLBuffer.h"

#include "glad/gl.h"

OpenGLVertexBuffer::OpenGLVertexBuffer(const float* vertices, const uint32_t size) {
#ifdef OPENGL_4_6
    glCreateBuffers(1, &this->id);
#else
    glGenBuffers(1, &this->id);
#endif
    glBindBuffer(GL_ARRAY_BUFFER, this->id);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer() {
    glDeleteBuffers(1, &this->id);
}

void OpenGLVertexBuffer::bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, this->id);
}


OpenGLIndexBuffer::OpenGLIndexBuffer(const uint32_t* indexes, const uint32_t count) : count(count) {
#ifdef OPENGL_4_6
    glCreateBuffers(1, &this->id);
#else
    glGenBuffers(1, &this->id);
#endif
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizei>(sizeof(uint32_t) * count), indexes, GL_STATIC_DRAW);
}

OpenGLIndexBuffer::~OpenGLIndexBuffer() {
    glDeleteBuffers(1, &this->id);
}

void OpenGLIndexBuffer::bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id);
}
