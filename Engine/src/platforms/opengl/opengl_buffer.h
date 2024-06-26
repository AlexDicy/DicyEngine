﻿#pragma once
#include "rendering/buffer.h"

class OpenGLVertexBuffer : public VertexBuffer {
public:
    OpenGLVertexBuffer(const float* vertices, uint32_t size);
    ~OpenGLVertexBuffer() override;

    void bind() const override;

    // make sure to also update the corresponding VertexArray to load the layout
    void set_layout(const BufferLayout& layout) override {
        this->layout = layout;
    }
    const BufferLayout& get_layout() const override {
        return this->layout;
    }

private:
    uint32_t id;
    BufferLayout layout;
};

class OpenGLIndexBuffer : public IndexBuffer {
public:
    OpenGLIndexBuffer(const uint32_t* indexes, uint32_t count);
    ~OpenGLIndexBuffer() override;

    void bind() const override;
    uint32_t get_count() const override {
        return this->count;
    }

private:
    uint32_t id;
    uint32_t count;
};
