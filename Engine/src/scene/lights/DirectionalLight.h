#pragma once

class DirectionalLight {
public:
    DirectionalLight(const Rotation& rotation, float intensity) : rotation(rotation), intensity(intensity) {}

    glm::vec3 getLocalDirection(const glm::mat4& transformMatrix) const {
        const auto world3x3 = glm::mat3(transformMatrix);
        const auto worldToLocal = glm::transpose(world3x3);
        return normalize(worldToLocal * this->rotation.toDirection());
    }

    float getIntensity() const {
        return this->intensity;
    }

    Rotation& getRotation() {
        return this->rotation;
    }

private:
    Rotation rotation = Rotation();
    float intensity = 1.0f;
};
