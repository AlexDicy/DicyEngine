#include "pch/enginepch.h"
#include "Image.h"


Image::Image(const unsigned int width, const unsigned int height, const TextureFormat format, const TextureInternalFormat internalFormat, std::unique_ptr<uint8_t[]> data) :
    width(width), height(height), format(format), internalFormat(internalFormat), data(std::move(data)) {}

Image::Image(const unsigned int width, const unsigned int height, const TextureFormat format, const TextureInternalFormat internalFormat) :
    width(width), height(height), format(format), internalFormat(internalFormat) {
    this->data = std::make_unique<uint8_t[]>(getDataSize());
}

void* Image::getPixelPointer(const unsigned int x, const unsigned int y) const {
    return this->data.get() + static_cast<size_t>((y * this->width + x) * this->internalFormat.getSize());
}
