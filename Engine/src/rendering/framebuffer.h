﻿#pragma once

class Framebuffer {
public:
    Framebuffer(const uint32_t width, const uint32_t height) : width(width), height(height) {}
    virtual ~Framebuffer() = default;

    uint32_t get_width() const {
        return this->width;
    }

    uint32_t get_height() const {
        return this->height;
    }

    virtual void bind() const = 0;

    virtual uint32_t get_color_texture_id() const = 0;
    virtual uint32_t get_depth_texture_id() const = 0;

private:
    uint32_t width;
    uint32_t height;
};
