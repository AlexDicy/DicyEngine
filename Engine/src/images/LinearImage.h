#pragma once

class LinearImage : public Image {
public:
    LinearImage() = default;
    explicit LinearImage(const std::string& path);

private:
    static void convertRGBEtoRGB(const unsigned char* rgbe, float* rgb);

    float gamma;
    float exposure;
    std::unique_ptr<float[]> data;
};
