#include "pch/enginepch.h"
#include "OpenGLVertexArray.h"
#include "OpenGLDataType.h"

#include <glad/gl.h>


OpenGLVertexArray::OpenGLVertexArray(const Ref<VertexBuffer>& vertexBuffer, const Ref<IndexBuffer>& indexBuffer) :
    vertexBuffer(vertexBuffer), indexBuffer(indexBuffer) {
#ifdef OPENGL_4_6
    glCreateVertexArrays(1, &this->id);
#else
    glGenVertexArrays(1, &this->id);
#endif

    glBindVertexArray(this->id);
    // vertex buffer
    vertexBuffer->bind();

    BufferLayout layout = vertexBuffer->getLayout();
    if (layout.getAttributes().empty()) {
        DE_WARN("Trying to set VertexBuffer in a VertexArray with an empty layout");
    }

    int layoutIndex = 0;
    for (auto& attribute : layout.getAttributes()) {
        glEnableVertexAttribArray(layoutIndex);
        // clang-format off
        glVertexAttribPointer(
            layoutIndex,
            attribute.getDatatypeCount(),
            OpenGLDataType::getFromDataType(attribute.type),
            attribute.isNormalized ? GL_TRUE : GL_FALSE,
            layout.getSize(), // NOLINT(bugprone-narrowing-conversions, cppcoreguidelines-narrowing-conversions)
            #pragma warning(suppress: 4312)
            reinterpret_cast<const void*>(attribute.offset) // NOLINT(performance-no-int-to-ptr)
        );
        // clang-format on
        layoutIndex++;
    }

    // index buffer
    indexBuffer->bind();

    this->OpenGLVertexArray::unbind();
    vertexBuffer->unbind();
    indexBuffer->unbind();
}

OpenGLVertexArray::~OpenGLVertexArray() {
    glDeleteVertexArrays(1, &this->id);
}

void OpenGLVertexArray::bind() const {
    glBindVertexArray(this->id);
}

void OpenGLVertexArray::unbind() const {
    glBindVertexArray(0);
}

const Ref<VertexBuffer>& OpenGLVertexArray::getVertexBuffer() const {
    return this->vertexBuffer;
}

const Ref<IndexBuffer>& OpenGLVertexArray::getIndexBuffer() const {
    return this->indexBuffer;
}
