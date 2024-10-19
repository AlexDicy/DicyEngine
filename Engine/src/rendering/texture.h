#pragma once

class Texture {
public:
    virtual ~Texture() = default;

    virtual void bind(uint32_t) const = 0;

    virtual uint32_t get_width() const = 0;
    virtual uint32_t get_height() const = 0;
};

class Texture2D : public Texture {
};

class TextureCube : public Texture {
    virtual uint32_t get_size() const = 0;
};
