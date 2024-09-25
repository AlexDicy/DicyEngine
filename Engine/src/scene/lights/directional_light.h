#pragma once

class DirectionalLight {
public:
    DirectionalLight(const Rotation& rotation, float intensity) : rotation(rotation), intensity(intensity) {}

    glm::vec3 get_local_direction(const glm::mat4& transform_matrix) const {
        const auto world3x3 = glm::mat3(transform_matrix);
        const auto world_to_local = transpose(world3x3);
        return normalize(world_to_local * this->rotation.to_direction());
    }

    float get_intensity() const {
        return this->intensity;
    }

    Rotation& get_rotation() {
        return this->rotation;
    }

private:
    Rotation rotation = Rotation();
    float intensity = 1.0f;
};
