#pragma once
#include "buffer.h"

class VertexArray {
public:
    virtual ~VertexArray() = default;

    virtual void bind() const = 0;

    virtual const std::shared_ptr<VertexBuffer>& get_vertex_buffer() const = 0;
    virtual const std::shared_ptr<IndexBuffer>& get_index_buffer() const = 0;

    virtual void set_vertex_buffer(const std::shared_ptr<VertexBuffer>& buffer) = 0;
    virtual void set_index_buffer(const std::shared_ptr<IndexBuffer>& buffer) = 0;
};
