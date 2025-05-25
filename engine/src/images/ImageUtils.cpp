#include "pch/enginepch.h"
#include "ImageUtils.h"

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
