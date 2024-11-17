#pragma once
#include <glm/gtx/quaternion.hpp>

/**
 * Rotation stores pitch, yaw and roll as degrees.
 * Positive values for pitch rotate X axis up, (up and down)
 * Positive values for yaw rotate Y axis right, (turning your head)
 * Positive values for roll rotate Z axis clockwise, (tilting your head)
 *
 * The actual rotation is stored as a quaternion, which is only updated when needed.
 */
class Rotation {
    friend class Transform;
    friend class CameraScript;
public:
    Rotation() : Rotation(0.0f, 0.0f, 0.0f) {}
    Rotation(const float pitch, const float yaw, const float roll) : pitch(pitch), yaw(yaw), roll(roll), quaternion() {}
    Rotation(const glm::quat& quaternion) : quaternion(quaternion) {
        this->recalculateAngles();
    }

    float getPitch() const {
        return this->pitch;
    }

    float getYaw() const {
        return this->yaw;
    }

    float getRoll() const {
        return this->roll;
    }

    glm::quat getQuaternion() {
        if (this->recalculateQuaternion) {
            this->recalculateQuaternion = false;
            this->quaternion = glm::quat(radians(glm::vec3(-this->pitch, this->yaw, -this->roll)));
        }
        return this->quaternion;
    }

    void setPitch(const float pitch) {
        this->pitch = pitch;
        this->recalculateQuaternion = true;
    }

    void setYaw(const float yaw) {
        this->yaw = yaw;
        this->recalculateQuaternion = true;
    }

    void setRoll(const float roll) {
        this->roll = roll;
        this->recalculateQuaternion = true;
    }

    glm::vec3 toDirection() {
        const glm::quat quaternion = this->getQuaternion();
        return normalize(quaternion * glm::vec3(0.0f, 0.0f, 1.0f));
    }

protected:
    bool needsRecalculation() const {
        return this->recalculateQuaternion;
    }

private:
    void recalculateAngles() {
        const glm::vec3 eulerAngles = degrees(glm::eulerAngles(this->quaternion));
        this->pitch = -eulerAngles.x;
        this->yaw = eulerAngles.y;
        this->roll = -eulerAngles.z;
    }

    //
    // Keep in sync with scripting/com.dicydev.engine.math.Rotation
    //

    float pitch;
    float yaw;
    float roll;
    glm::quat quaternion;
    bool recalculateQuaternion = true;
};
