#include "pch/enginepch.h"
#include "opengl_vertex_array.h"

#include "platforms/opengl/opengl_datatype.h"

#include "glad/gl.h"


OpenGLVertexArray::OpenGLVertexArray(const std::shared_ptr<VertexBuffer>& vertex_buffer, const std::shared_ptr<IndexBuffer>& index_buffer) :
    vertex_buffer(vertex_buffer), index_buffer(index_buffer) {
    glCreateVertexArrays(1, &this->id);

    glBindVertexArray(this->id);
    // vertex buffer
    vertex_buffer->bind();

    BufferLayout layout = vertex_buffer->get_layout();
    if (layout.get_attributes().empty()) {
        DE_WARN("Trying to set VertexBuffer in a VertexArray with an empty layout");
    }

    int layout_index = 0;
    for (auto& attribute : layout.get_attributes()) {
        glEnableVertexAttribArray(layout_index);
        // clang-format off
        glVertexAttribPointer(
            layout_index,
            attribute.get_datatype_count(),
            get_opengl_type_from_datatype(attribute.type),
            attribute.is_normalized ? GL_TRUE : GL_FALSE,
            layout.get_size(), // NOLINT(bugprone-narrowing-conversions, cppcoreguidelines-narrowing-conversions)
            #pragma warning(suppress: 4312)
            reinterpret_cast<const void*>(attribute.offset) // NOLINT(performance-no-int-to-ptr)
        );
        // clang-format on
        layout_index++;
    }

    // index buffer
    index_buffer->bind();
    glBindVertexArray(0);
}

OpenGLVertexArray::~OpenGLVertexArray() {
    glDeleteVertexArrays(1, &this->id);
}

void OpenGLVertexArray::bind() const {
    glBindVertexArray(this->id);
}

const std::shared_ptr<VertexBuffer>& OpenGLVertexArray::get_vertex_buffer() const {
    return this->vertex_buffer;
}

const std::shared_ptr<IndexBuffer>& OpenGLVertexArray::get_index_buffer() const {
    return this->index_buffer;
}
