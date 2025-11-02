#include "pch/enginepch.h"
#include "ImageUtils.h"

#include <stb_image.h>

Ref<Image> ImageUtils::loadImageFromFile(const std::string& path) {
    unsigned int width;
    unsigned int height;
    TextureFormat format;
    TextureInternalFormat internalFormat;
    std::unique_ptr<uint8_t[]> data = loadImageData(path, width, height, format, internalFormat);
    return std::make_shared<Image>(width, height, format, internalFormat, std::move(data));
}

Ref<Texture> ImageUtils::loadTextureFromFile(const Ref<Renderer>& renderer, const std::string& path) {
    unsigned int width;
    unsigned int height;
    TextureFormat format;
    TextureInternalFormat internalFormat;
    std::unique_ptr<uint8_t[]> data = loadImageData(path, width, height, format, internalFormat);
    return Texture::builder() //
        .width(width)
        .height(height)
        .format(format)
        .internalFormat(internalFormat)
        .type(TextureType::TEXTURE_2D)
        .data(std::move(data))
        .build(renderer);
}

/**
 * Always remember to free the returned data using stbi_image_free when done using it.
 */
std::unique_ptr<uint8_t[]> ImageUtils::loadImageData(const std::string& path, unsigned int& width, unsigned int& height, TextureFormat& format,
                                                     TextureInternalFormat& internalFormat) {
    int w, h;
    int channels;

    stbi_set_flip_vertically_on_load(true);
    uint8_t* data = stbi_load(path.c_str(), &w, &h, &channels, 0);

    if (!data) {
        const char* error = stbi_failure_reason();
        DE_ERROR("Failed to read texture file {0} - {1}", path, error);
    }

    width = static_cast<unsigned int>(w);
    height = static_cast<unsigned int>(h);
    format = channels > 3 ? TextureFormat::RGBA : TextureFormat::RGB;
    internalFormat = channels > 3 ? TextureInternalFormat::RGBA8 : TextureInternalFormat::RGB8;
    return std::unique_ptr<uint8_t[]>(data);
}

Ref<LinearImage> ImageUtils::acesFilmicTonemapping(const Ref<LinearImage>& image) {
    auto result = std::make_shared<LinearImage>(image->getWidth(), image->getHeight());
    for (unsigned int x = 0; x < image->getWidth(); x++) {
        for (unsigned int y = 0; y < image->getHeight(); y++) {
            const auto originalPixel = static_cast<float*>(image->getPixelPointer(x, y));
            const auto resultPixel = static_cast<float*>(result->getPixelPointer(x, y));
            const auto color = glm::vec3(originalPixel[0], originalPixel[1], originalPixel[2]);
            const auto alpha = originalPixel[3];
            constexpr float a = 2.51f;
            constexpr float b = 0.03f;
            constexpr float c = 2.43f;
            constexpr float d = 0.59f;
            constexpr float e = 0.14f;
            const glm::vec3 mapped = glm::clamp((color * (a * color + b)) / (color * (c * color + d) + e), glm::vec3(0.0), glm::vec3(1.0));
            *reinterpret_cast<glm::vec4*>(resultPixel) = glm::vec4(mapped, alpha);
        }
    }
    return result;
}
