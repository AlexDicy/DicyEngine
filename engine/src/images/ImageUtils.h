#pragma once
#include "LinearImage.h"

class ImageUtils {
public:
    static Ref<Image> loadImageFromFile(const std::string& path);
    static Ref<Texture> loadTextureFromFile(const Ref<Renderer>& renderer, const std::string& path);
    static std::unique_ptr<uint8_t[]> loadImageData(const std::string& path, unsigned int& width, unsigned int& height, TextureFormat& format, TextureInternalFormat& internalFormat);
    static Ref<LinearImage> acesFilmicTonemapping(const Ref<LinearImage>& image);

private:
    static Ref<Image> createImageFromData(unsigned int width, unsigned int height, TextureFormat format, TextureInternalFormat internalFormat, std::unique_ptr<uint8_t[]> data);
    static Ref<Texture> createTextureFromData(const Ref<Renderer>& renderer, unsigned int width, unsigned int height, TextureFormat format,
                                              TextureInternalFormat internalFormat, std::unique_ptr<uint8_t[]> data);
};
