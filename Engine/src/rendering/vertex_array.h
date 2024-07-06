#pragma once
#include "buffer.h"

class VertexArray {
public:
    virtual ~VertexArray() = default;

    virtual void bind() const = 0;

    virtual const Ref<VertexBuffer>& get_vertex_buffer() const = 0;
    virtual const Ref<IndexBuffer>& get_index_buffer() const = 0;
};
