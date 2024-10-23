#pragma once
#include <glm/gtx/quaternion.hpp>

/**
 * Rotation stores pitch, yaw and roll as degrees.
 * Positive values for pitch rotate X axis up, (up and down)
 * Positive values for yaw rotate Y axis right, (turning your head)
 * Positive values for roll rotate Z axis clockwise, (tilting your head)
 */
struct Rotation {
    float pitch;
    float yaw;
    float roll;

    Rotation() : pitch(0.0f), yaw(0.0f), roll(0.0f) {}
    Rotation(const float pitch, const float yaw, const float roll) : pitch(pitch), yaw(yaw), roll(roll) {}

    glm::quat toQuaternion() const {
        return {radians(glm::vec3(-this->pitch, this->yaw, -this->roll))};
    }

    glm::vec3 toDirection() const {
        const glm::quat quaternion = this->toQuaternion();
        return normalize(quaternion * glm::vec3(0.0f, 0.0f, 1.0f));
    }
};
