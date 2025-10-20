#pragma once
#include "Texture.h"

class FramebufferBuilder;

class Framebuffer : public std::enable_shared_from_this<Framebuffer> {
public:
    virtual ~Framebuffer();

    unsigned int getWidth() const {
        return this->params.width;
    }

    unsigned int getHeight() const {
        return this->params.height;
    }

    unsigned int getSamples() const {
        return this->params.samples;
    }

    void bind() const;


    struct FramebufferParams {
        unsigned int width = 0;
        unsigned int height = 0;
        unsigned int samples = 1;
        std::vector<Ref<Texture>> colorAttachments;
        Ref<Texture> depthAttachment = nullptr;
    };

protected:
    Framebuffer(const FramebufferParams& params, const Ref<Renderer>& renderer);

    Ref<Renderer> getRenderer() const {
        Ref<Renderer> ref = renderer.lock();
        if (!ref) {
            throw std::runtime_error("The renderer associated with this texture has been destroyed.");
        }
        return ref;
    }

    FramebufferParams params;

private:
    std::weak_ptr<Renderer> renderer;
};


class FramebufferBuilder {
public:
    FramebufferBuilder& width(const unsigned int width) {
        this->params.width = width;
        return *this;
    }

    FramebufferBuilder& height(const unsigned int height) {
        this->params.height = height;
        return *this;
    }

    FramebufferBuilder& size(const unsigned int size) {
        this->width(size);
        this->height(size);
        return *this;
    }

    FramebufferBuilder& size(const unsigned int width, const unsigned int height) {
        this->width(width);
        this->height(height);
        return *this;
    }

    FramebufferBuilder& samples(const unsigned int samples) {
        this->params.samples = samples;
        return *this;
    }

    FramebufferBuilder& addColorAttachment(const Ref<Texture>& texture) {
        this->params.colorAttachments.push_back(texture);
        return *this;
    }

    FramebufferBuilder& depthAttachment(const Ref<Texture>& texture) {
        this->params.depthAttachment = texture;
        return *this;
    }

    Ref<Framebuffer> build(const Ref<Renderer>& renderer) const;

private:
    Framebuffer::FramebufferParams params;
};
