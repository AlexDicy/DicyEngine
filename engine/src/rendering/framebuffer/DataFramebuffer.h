#pragma once

class DataFramebuffer {
public:
    DataFramebuffer(const unsigned int width, const unsigned int height) : width(width), height(height) {}
    virtual ~DataFramebuffer() = default;

    unsigned int getWidth() const {
        return this->width;
    }

    unsigned int getHeight() const {
        return this->height;
    }

    virtual void bind() const = 0;
    virtual void clear() const = 0;

    virtual const Ref<Texture>& getDataTexture() const = 0;

protected:
    unsigned int width;
    unsigned int height;
};
