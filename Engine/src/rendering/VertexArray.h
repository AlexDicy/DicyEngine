#pragma once
#include "Buffer.h"

class VertexArray {
public:
    virtual ~VertexArray() = default;

    virtual void bind() const = 0;

    virtual const Ref<VertexBuffer>& getVertexBuffer() const = 0;
    virtual const Ref<IndexBuffer>& getIndexBuffer() const = 0;
};
