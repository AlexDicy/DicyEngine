#pragma once
#include "rendering/framebuffer.h"

class OpenGLFramebuffer : public Framebuffer {
public:
    OpenGLFramebuffer(uint32_t width, uint32_t height);
    ~OpenGLFramebuffer() override;

    void bind() const override;

    uint32_t get_color_texture_id() const override {
        return this->color_texture_id;
    }

    uint32_t get_depth_texture_id() const override {
        return this->depth_texture_id;
    }

private:
    uint32_t id;
    uint32_t color_texture_id;
    uint32_t depth_texture_id;
};
