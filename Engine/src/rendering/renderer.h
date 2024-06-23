#pragma once
#include "buffer.h"
#include "vertex_array.h"

enum class RenderAPI { NONE, OPENGL };

class Renderer {
public:
    explicit Renderer(const RenderAPI api) : api(api) {}
    virtual ~Renderer() = default;

    RenderAPI get_api() const {
        return this->api;
    }

    virtual VertexArray* create_vertex_array(const std::shared_ptr<VertexBuffer>& vertex_buffer, const std::shared_ptr<IndexBuffer>& index_buffer) const = 0;
    virtual VertexBuffer* create_vertex_buffer(const float* vertices, uint32_t size) const = 0;
    virtual IndexBuffer* create_index_buffer(const uint32_t* indexes, uint32_t count) const = 0;

    virtual void clean() const = 0;
    virtual void draw(const std::shared_ptr<VertexArray>& vertex_array) const = 0;

private:
    RenderAPI api;
};
