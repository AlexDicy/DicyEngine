#pragma once
#include "rendering/vertex_array.h"

class OpenGLVertexArray : public VertexArray {
public:
    OpenGLVertexArray();
    ~OpenGLVertexArray() override;

    void bind() const override;

    const std::shared_ptr<VertexBuffer>& get_vertex_buffer() const override;
    const std::shared_ptr<IndexBuffer>& get_index_buffer() const override;

    void set_vertex_buffer(const std::shared_ptr<VertexBuffer>& buffer) override;
    void set_index_buffer(const std::shared_ptr<IndexBuffer>& buffer) override;

private:
    std::shared_ptr<VertexBuffer> vertex_buffer;
    std::shared_ptr<IndexBuffer> index_buffer;
    uint32_t id;
};
