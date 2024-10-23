#pragma once
#include "rendering/vertex_array.h"

class OpenGLVertexArray : public VertexArray {
public:
    OpenGLVertexArray(const Ref<VertexBuffer>& vertexBuffer, const Ref<IndexBuffer>& indexBuffer);
    ~OpenGLVertexArray() override;

    void bind() const override;

    const Ref<VertexBuffer>& getVertexBuffer() const override;
    const Ref<IndexBuffer>& getIndexBuffer() const override;

private:
    Ref<VertexBuffer> vertexBuffer;
    Ref<IndexBuffer> indexBuffer;
    uint32_t id;
};
