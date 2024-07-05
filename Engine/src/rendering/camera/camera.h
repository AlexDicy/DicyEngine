#pragma once

#include "math/rotation.h"
#include <glm/glm.hpp>


class Camera {
public:
    virtual ~Camera() = default;
    explicit Camera(const glm::mat4& projection_matrix);

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
    void set_rotation(const Rotation& rotation);
    void set_pitch(float pitch);
    void set_yaw(float yaw);
    void update_view_matrix();

    virtual void set_aspect_ratio(float aspect_ratio) = 0;

protected:
    glm::vec3 position = glm::vec3(0.0f);
    Rotation rotation;

    glm::mat4 view_matrix;
    glm::mat4 projection_matrix;
    glm::mat4 view_projection_matrix;
};
