#pragma once

#include <glm/glm.hpp>


class Camera {
public:
    Camera(float left, float right, float bottom, float top);

    const glm::vec3& get_position() const {
        return this->position;
    }

    float get_rotation() const {
        return this->rotation_z;
    }

    const glm::mat4& get_view_matrix() const {
        return this->view_matrix;
    }

    const glm::mat4& get_projection_matrix() const {
        return this->projection_matrix;
    }

    const glm::mat4& get_view_projection_matrix() const {
        return this->view_projection_matrix;
    }

    void set_position(const glm::vec3& position);
    void set_rotation(float rotation);
    void update_view_matrix();

private:
    glm::vec3 position = glm::vec3(0.0f);
    float rotation_z = 0.0f;
    glm::mat4 view_matrix;
    glm::mat4 projection_matrix;
    glm::mat4 view_projection_matrix;
};
