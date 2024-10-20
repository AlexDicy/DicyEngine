#include "pch/enginepch.h"
#include "CubeMap.h"


CubeMap::CubeMap(std::array<Image, 6> faces) : faces(std::move(faces)) {
    this->face_size = this->faces[0].getWidth(); // Assuming all faces have the same size and are square
}

glm::vec3 CubeMap::getPixel(const Face face, const unsigned int x, const unsigned int y) const {
    void* pixel = faces[static_cast<uint8_t>(face)].getPixel(x, y);
    return {static_cast<uint8_t*>(pixel)[0], static_cast<uint8_t*>(pixel)[1], static_cast<uint8_t*>(pixel)[2]};
}

glm::vec3 CubeMap::getPixelDirection(const Face face, const unsigned int x, const unsigned int y) const {
    // Normalize x and y to [-1, 1]
    const float normalizedX = 2.0f * static_cast<float>(x) / static_cast<float>(this->face_size) - 1.0f;
    const float normalizedY = 2.0f * static_cast<float>(y) / static_cast<float>(this->face_size) - 1.0f;

    switch (face) {
        case Face::RIGHT:
            return {1.0f, normalizedY, -normalizedX};
        case Face::LEFT:
            return {-1.0f, normalizedY, normalizedX};
        case Face::TOP:
            return {normalizedX, 1.0f, -normalizedY};
        case Face::BOTTOM:
            return {normalizedX, -1.0f, normalizedY};
        case Face::FRONT:
            return {normalizedX, normalizedY, 1.0f};
        case Face::BACK:
            return {-normalizedX, normalizedY, -1.0f};
    }
    throw std::invalid_argument("Invalid CubeMap::Face for getPixelDirection");
}
