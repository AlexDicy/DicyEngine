#pragma once

class DirectionalLight {
public:
    DirectionalLight(const Rotation& rotation, float intensity) : rotation(rotation), intensity(intensity) {}

    glm::vec3 getLocalDirection(const glm::mat4& transformMatrix) {
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

    glm::mat4 getViewProjectionMatrix() {
        const glm::mat4 projection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 1000.0f);
        const glm::mat4 view = glm::lookAt(-this->rotation.toDirection() * 100.0f, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        return projection * view;
    }

private:
    Rotation rotation = Rotation();
    float intensity = 1.0f;
};
