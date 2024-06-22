#pragma once
#include "buffer_layout.h"

class VertexBuffer {
public:
    virtual ~VertexBuffer() = default;

    virtual void bind() const = 0;

    // make sure to also update the corresponding VertexArray to load the layout
    virtual void set_layout(const BufferLayout& layout) = 0;
    virtual const BufferLayout& get_layout() const = 0;
};

class IndexBuffer {
public:
    virtual ~IndexBuffer() = default;

    virtual void bind() const = 0;
    virtual uint32_t get_count() const = 0;
};
