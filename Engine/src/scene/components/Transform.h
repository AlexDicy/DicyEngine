#pragma once
#include "math/Rotation.h"

struct Transform {
    glm::vec3 position;
    Rotation rotation;
    glm::vec3 scale;

    Transform(const glm::vec3 position, const Rotation rotation, const glm::vec3 scale) : position(position), rotation(rotation), scale(scale) {}
    Transform(const glm::vec3 position, const Rotation rotation) : Transform(position, rotation, glm::vec3(1.0f)) {}
    Transform(const glm::vec3 position) : Transform(position, Rotation(), glm::vec3(1.0f)) {}
    Transform() : Transform(glm::vec3(0.0f)) {}
};
