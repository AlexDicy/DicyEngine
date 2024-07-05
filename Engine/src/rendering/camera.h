#pragma once

#include "math/rotation.h"
#include <glm/glm.hpp>


class Camera {
public:
    Camera(float left, float right, float bottom, float top);
    Camera(float fov_y, float aspect_ratio);

    const glm::vec3& get_position() const {
        return this->position;
    }

    float get_pitch() const {
        return this->rotation.pitch;
    }

    float get_yaw() const {
        return this->rotation.yaw;
    }

    const Rotation& get_rotation() const {
        return this->rotation;
    }

    const glm::mat4& get_view_projection_matrix(const bool recalculate) {
        if (recalculate) {
            this->update_view_matrix();
        }
        return this->view_projection_matrix;
    }

    void set_position(const glm::vec3& position);
    void set_pitch(float pitch);
    void set_yaw(float yaw);
    void set_rotation(const Rotation& rotation);
    void update_view_matrix();

private:
    glm::vec3 position = glm::vec3(0.0f);
    Rotation rotation;

    glm::mat4 view_matrix;
    glm::mat4 projection_matrix;
    glm::mat4 view_projection_matrix;
};
