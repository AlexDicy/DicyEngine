#pragma once
#include "math/rotation.h"

struct Transform {
    glm::vec3 position;
    Rotation rotation;
    glm::vec3 scale;
};
