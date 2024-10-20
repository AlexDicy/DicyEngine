#include "pch/enginepch.h"
#include "Image.h"

#include "stb_image_write.h"

#include <stb_image.h>

Image::Image(const std::string& path) {
    const bool is_hdr = path.ends_with(".hdr");
    int width;
    int height;
    int channels;

    stbi_set_flip_vertically_on_load(true);
    void* texture;
    if (is_hdr) {
        texture = stbi_loadf(path.c_str(), &width, &height, &channels, 0);
    } else {
        texture = stbi_load(path.c_str(), &width, &height, &channels, 0);
    }

    if (!texture) {
        const char* error = stbi_failure_reason();
        DE_ERROR("Failed to read texture file {0} - {1}", path, error);
    }

    this->width = width;
    this->height = height;
    this->channels = channels;
    this->bytesPerPixel = is_hdr ? sizeof(float) : sizeof(uint8_t);
    this->data = std::make_unique<uint8_t[]>(width * height * channels * bytesPerPixel);
    memcpy(this->data.get(), texture, width * height * channels * bytesPerPixel);

    stbi_image_free(texture);
}

Image::Image(const unsigned int width, const unsigned int height, const unsigned int channels, const unsigned int bytesPerPixel, const void* data) : width(width), height(height), channels(channels), bytesPerPixel(bytesPerPixel) {
    this->data = std::make_unique<uint8_t[]>(width * height * channels * bytesPerPixel);
    if (data) {
        memcpy(this->data.get(), data, width * height * channels * bytesPerPixel);
    }
}

void* Image::getPixelPointer(const unsigned int x, const unsigned int y) const {
    return this->data.get() + static_cast<size_t>((y * this->width + x) * this->channels * this->bytesPerPixel);
}
