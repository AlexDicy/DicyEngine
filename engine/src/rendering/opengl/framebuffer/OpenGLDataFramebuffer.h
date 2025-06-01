#pragma once
#include "rendering/framebuffer/DataFramebuffer.h"

class OpenGLDataFramebuffer : public DataFramebuffer {
public:
    OpenGLDataFramebuffer(unsigned int width, unsigned int height);
    ~OpenGLDataFramebuffer() override;

    void bind() const override;
    void clear() const override;

    const Ref<Texture2D>& getDataTexture() const override {
        return this->depthTexture;
    }

private:
    unsigned int id;
    Ref<Texture2D> depthTexture;
};
