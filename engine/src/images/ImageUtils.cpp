#include "pch/enginepch.h"
#include "ImageUtils.h"

#include <stb_image.h>

Ref<Texture> ImageUtils::loadTextureFromFile(const Ref<Renderer>& renderer, const std::string& path) {
    const bool isHDR = path.ends_with(".hdr");
    int width;
    int height;
    int channels;

    stbi_set_flip_vertically_on_load(true);
    void* data;
    if (isHDR) {
        data = stbi_loadf(path.c_str(), &width, &height, &channels, 0);
    } else {
        data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    }

    if (!data) {
        const char* error = stbi_failure_reason();
        DE_ERROR("Failed to read texture file {0} - {1}", path, error);
    }

    const Texture::Format format = !isHDR && channels > 3 ? Texture::Format::RGBA : Texture::Format::RGB;
    const Texture::InternalFormat internalFormat = isHDR ? (channels > 3 ? Texture::InternalFormat::RGBA16F : Texture::InternalFormat::RGB16F)
                                                         : (channels > 3 ? Texture::InternalFormat::RGBA8 : Texture::InternalFormat::RGB8);
    Ref<Texture> texture = Texture::builder() //
                               .setWidth(width)
                               .setHeight(height)
                               .setFormat(format)
                               .setInternalFormat(internalFormat)
                               .setData(data)
                               .setSourcePath(path)
                               .build(renderer);
    stbi_image_free(data);
    return texture;
}

Ref<LinearImage> ImageUtils::acesFilmicTonemapping(const Ref<LinearImage>& image) {
    auto result = std::make_shared<LinearImage>(image->getWidth(), image->getHeight(), nullptr);
    for (unsigned int x = 0; x < image->getWidth(); x++) {
        for (unsigned int y = 0; y < image->getHeight(); y++) {
            const auto originalPixel = static_cast<float*>(image->getPixelPointer(x, y));
            const auto resultPixel = static_cast<float*>(result->getPixelPointer(x, y));
            const auto color = glm::vec3(originalPixel[0], originalPixel[1], originalPixel[2]);
            constexpr float a = 2.51f;
            constexpr float b = 0.03f;
            constexpr float c = 2.43f;
            constexpr float d = 0.59f;
            constexpr float e = 0.14f;
            *reinterpret_cast<glm::vec3*>(resultPixel) = glm::clamp((color * (a * color + b)) / (color * (c * color + d) + e), glm::vec3(0.0), glm::vec3(1.0));
        }
    }
    return result;
}
