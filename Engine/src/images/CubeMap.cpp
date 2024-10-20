#include "pch/enginepch.h"
#include "CubeMap.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


CubeMap::CubeMap(std::array<Image, 6> faces) : faces(std::move(faces)) {
    this->face_size = this->faces[0].getWidth(); // Assuming all faces have the same size and are square
}

const Image& CubeMap::getFace(Face face) const {
    return faces[static_cast<uint8_t>(face)];
}

glm::vec3 CubeMap::getPixel(const Face face, const unsigned int x, const unsigned int y) const {
    auto pixel = static_cast<float*>(faces[static_cast<uint8_t>(face)].getPixelPointer(x, y));
    return {pixel[0], pixel[1], pixel[2]};
}

void* CubeMap::getPixelPointer(Face face, unsigned int x, unsigned int y) const {
    return faces[static_cast<uint8_t>(face)].getPixelPointer(x, y);
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

// void CubeMap::save_to_files(const std::string& filenamePrefix) {
//     std::string facesSuffix[] = {"right", "left", "top", "bottom", "front", "back"};
//     for (uint8_t i = 0; i < 6; i++) {
//         facesSuffix[i] = filenamePrefix + "_" + facesSuffix[i] + ".hdr";
//         void* data = faces[i].getData();
//         stbi_write_hdr(facesSuffix[i].c_str(), faces[i].getWidth(), faces[i].getHeight(), faces[i].getChannels(), reinterpret_cast<const float*>(data));
//     }
// }
