#pragma once
#include "Image.h"

class CubeMap {
public:
    enum class Face : uint8_t {
        RIGHT = 0, //             +----+
        LEFT = 1, //              | 2  |
        TOP = 2, //          +----+----+----+----+
        BOTTOM = 3, //       | 1  | 4  | 0  | 5  |
        FRONT = 4, //        +----+----+----+----+
        BACK = 5, //              | 3  |
        //                        +----+
    };

    explicit CubeMap(std::array<Image, 6> faces);

    glm::vec3 getPixel(Face face, unsigned int x, unsigned int y) const;
    glm::vec3 getPixelDirection(Face face, unsigned int x, unsigned int y) const;

private:
    std::array<Image, 6> faces;
    unsigned int face_size;
};
