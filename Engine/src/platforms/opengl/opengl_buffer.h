#pragma once
#include "rendering/buffer.h"

class OpenGLVertexBuffer : public Buffer {
public:
    OpenGLVertexBuffer(const float* vertices, uint32_t size);
    ~OpenGLVertexBuffer() override;

    void bind() const override;

private:
    uint32_t id;
};

class OpenGLIndexBuffer : public Buffer {
public:
    OpenGLIndexBuffer(const uint32_t* indexes, uint32_t count);
    ~OpenGLIndexBuffer() override;

    void bind() const override;

private:
    uint32_t id;
};
