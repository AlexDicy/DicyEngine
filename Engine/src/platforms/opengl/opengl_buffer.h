#pragma once
#include "rendering/buffer.h"

class OpenGLVertexBuffer : public VertexBuffer {
    uint32_t id;
    BufferLayout layout;

public:
    OpenGLVertexBuffer(const float* vertices, uint32_t size);
    ~OpenGLVertexBuffer() override;

    void bind() const override;
    void set_layout(const BufferLayout& layout) override {
        this->layout = layout;
    }
    const BufferLayout& get_layout() const override {
        return this->layout;
    }
};

class OpenGLIndexBuffer : public IndexBuffer {
public:
    OpenGLIndexBuffer(const uint32_t* indexes, uint32_t count);
    ~OpenGLIndexBuffer() override;

    void bind() const override;

private:
    uint32_t id;
};
