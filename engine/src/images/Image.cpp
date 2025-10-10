#include "pch/enginepch.h"
#include "Image.h"


Image::Image(const unsigned int width, const unsigned int height, const TextureFormat format, const TextureInternalFormat internalFormat, const void* data) :
    width(width), height(height), format(format), internalFormat(internalFormat) {
    this->data = std::make_unique<uint8_t[]>(this->getDataSize());
    if (data) {
        memcpy(this->data.get(), data, this->getDataSize());
    }
}

void* Image::getPixelPointer(const unsigned int x, const unsigned int y) const {
    return this->data.get() + static_cast<size_t>((y * this->width + x) * this->internalFormat.getSize());
}
