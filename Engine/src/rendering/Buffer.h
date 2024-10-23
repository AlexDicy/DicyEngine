#pragma once
#include "BufferLayout.h"

class VertexBuffer {
public:
    virtual ~VertexBuffer() = default;

    virtual void bind() const = 0;

    // make sure to also update the corresponding VertexArray to load the layout
    virtual void setLayout(const BufferLayout& layout) = 0;
    virtual const BufferLayout& getLayout() const = 0;
};

class IndexBuffer {
public:
    virtual ~IndexBuffer() = default;

    virtual void bind() const = 0;
    virtual uint32_t getCount() const = 0;
};
