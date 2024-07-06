#pragma once
#include "rendering/vertex_array.h"

class OpenGLVertexArray : public VertexArray {
public:
    OpenGLVertexArray(const Ref<VertexBuffer>& vertex_buffer, const Ref<IndexBuffer>& index_buffer);
    ~OpenGLVertexArray() override;

    void bind() const override;

    const Ref<VertexBuffer>& get_vertex_buffer() const override;
    const Ref<IndexBuffer>& get_index_buffer() const override;

private:
    Ref<VertexBuffer> vertex_buffer;
    Ref<IndexBuffer> index_buffer;
    uint32_t id;
};
