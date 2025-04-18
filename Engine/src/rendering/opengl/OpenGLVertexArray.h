﻿#pragma once
#include "rendering/VertexArray.h"

class OpenGLVertexArray : public VertexArray {
public:
    OpenGLVertexArray(const Ref<VertexBuffer>& vertexBuffer, const Ref<IndexBuffer>& indexBuffer);
    ~OpenGLVertexArray() override;

    void bind() const override;
    void unbind() const override;

    const Ref<VertexBuffer>& getVertexBuffer() const override;
    const Ref<IndexBuffer>& getIndexBuffer() const override;

private:
    Ref<VertexBuffer> vertexBuffer;
    Ref<IndexBuffer> indexBuffer;
    uint32_t id;
};
