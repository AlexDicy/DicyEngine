#pragma once
#include "rendering/Texture.h"

class OpenGLTexture2D : public Texture2D {
public:
    explicit OpenGLTexture2D(const std::string& path);
    OpenGLTexture2D(unsigned int channels, unsigned int width, unsigned int height, unsigned int bytesPerPixel, int format, const void* data);
    OpenGLTexture2D(unsigned int channels, unsigned int width, unsigned int height, unsigned int bytesPerPixel, const void* data);
    OpenGLTexture2D(unsigned int id, unsigned int width, unsigned int height);
    ~OpenGLTexture2D() override;

    void bind(uint32_t) const override;

    void setRawData(const void* data) override;
    void resize(unsigned width, unsigned height) override;

    void createTextureWithData(unsigned int channels, unsigned int width, unsigned int height, bool isHDR, const void* data);
    void createTextureWithData(unsigned int channels, unsigned int width, unsigned int height, bool isHDR, int format, const void* data);

private:
    std::string path;
    uint32_t id;

    // OpenGL specific
    int internalFormat;
    int format;
    int type;
};
