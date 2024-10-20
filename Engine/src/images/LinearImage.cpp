#include "pch/enginepch.h"
#include "LinearImage.h"

#include <fstream>

LinearImage::LinearImage(const std::string& path) {
    auto imageStream = std::ifstream(path, std::ios::in | std::ios::binary);
    if (!imageStream.is_open()) {
        DE_ERROR("Failed to open image file {0}", path);
    }

    float gamma = 1.0f;
    float exposure = 1.0f;
    char xSign = 0;
    char ySign = 0;
    unsigned int width = 0;
    unsigned int height = 0;

    std::string line;
    while (std::getline(imageStream, line, '\n')) {
        if (line[0] == '#') {
            continue;
        }

        sscanf(line.c_str(), "GAMMA=%f", &gamma); // NOLINT
        sscanf(line.c_str(), "EXPOSURE=%f", &exposure); // NOLINT
        if (sscanf(line.c_str(), "%cY %u %cX %u", &ySign, &height, &xSign, &width) == 4) { // NOLINT
            break;
        }
        if (sscanf(line.c_str(), "%cX %u %cY %u", &xSign, &width, &ySign, &height) == 4) { // NOLINT
            break;
        }
    }

    this->gamma = gamma;
    this->exposure = exposure;
    this->width = width;
    this->height = height;

    if (width == 0 || height == 0) {
        DE_ERROR("Failed to read image file {0}", path);
        return;
    }

    if (xSign == '-' || ySign == '+') {
        DE_ERROR("Unsupported image orientation");
        return;
    }

    this->data = std::make_unique<float[]>(width * height * 3);

    std::vector<unsigned char> row;
    row.reserve(width * 4); // RGBE

    // read the first 3 bytes of the file to check if the file is using RLE compression
    const std::streampos position = imageStream.tellg();
    imageStream.read(reinterpret_cast<char*>(row.data()), 3);
    imageStream.seekg(position);

    if (row[0] == 0x02 && row[1] == 0x02 && (row[2] & 0x80) == 0 && width >= 8 && width < 32768) {
        // the file is using RLE compression
        for (unsigned int y = 0; y < height; y++) {
            char magic[2];
            imageStream.read(magic, 2);
            if (magic[0] != 0x02 || magic[1] != 0x02) {
                DE_ERROR("Invalid magic number in RLE");
                return;
            }

            int len; // TODO: this is not working
            imageStream.read(reinterpret_cast<char*>(&len), 1);
            len <<= 8;
            unsigned char len2;
            imageStream.read(reinterpret_cast<char*>(&len2), 1);
            DE_INFO("len: {0}, len2: {1}, len |= len2: {2}", len, len2, len |= len2);
            len |= len2; // changed the order of the bytes
            if (len != width) {
                DE_ERROR("Invalid scanline length in RLE");
                return;
            }

            for (unsigned int k = 0; k < 4; ++k) {
                unsigned int nleft;
                unsigned int i = 0;
                while ((nleft = width - i) > 0) {
                    unsigned char count;
                    imageStream.read(reinterpret_cast<char*>(&count), 1);
                    if (count > 128) {
                        // Run
                        unsigned char value;
                        imageStream.read(reinterpret_cast<char*>(&value), 1);
                        count -= 128;
                        if (count == 0 || count > nleft) {
                            DE_ERROR("Invalid RLE data");
                            return;
                        }
                        for (unsigned int z = 0; z < count; ++z) {
                            row[i++ * 4 + k] = value;
                        }
                    } else {
                        // Dump
                        if (count == 0 || count > nleft) {
                            DE_ERROR("Invalid RLE data");
                            return;
                        }
                        for (unsigned int z = 0; z < count; ++z) {
                            imageStream.read(reinterpret_cast<char*>(&row[i++ * 4 + k]), 1);
                        }
                    }
                }
            }
            for (unsigned int x = 0; x < width; x++) {
                convertRGBEtoRGB(row.data() + x * 4, this->data.get() + (y * width + x) * 3);
            }
        }
    } else {
        throw std::runtime_error("NOT IMPLEMENTED");
    }
}

void LinearImage::convertRGBEtoRGB(const unsigned char* rgbe, float* rgb) {
    if (rgbe[3] == 0) {
        rgb[0] = rgb[1] = rgb[2] = 0;
    } else {
        const float exponent = std::ldexp(1.0f, rgbe[3] - (128 + 8));
        rgb[0] = rgbe[0] * exponent;
        rgb[1] = rgbe[1] * exponent;
        rgb[2] = rgbe[2] * exponent;
    }
}
