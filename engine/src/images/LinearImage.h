#pragma once
#include "Image.h"

class LinearImage final : public Image {
public:
    LinearImage() = default;
    explicit LinearImage(const std::string& path);
    LinearImage(unsigned int width, unsigned int height, std::unique_ptr<uint8_t[]> data, float gamma = 1.0f, float exposure = 1.0f);
    LinearImage(unsigned int width, unsigned int height, float gamma = 1.0f, float exposure = 1.0f);


private:
    static void convertRGBEtoRGB(const unsigned char* rgbe, float* rgb);

    float gamma;
    float exposure;
};
