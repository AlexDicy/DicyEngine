#include "pch/enginepch.h"
#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

Camera::Camera(const float left, const float right, const float bottom, const float top) : projection_matrix(glm::ortho(left, right, bottom, top, 0.1f, 1000.0f)) {
    update_view_matrix();
}

Camera::Camera(const float fov_y, const float aspect_ratio) : projection_matrix(glm::perspective(glm::radians(fov_y), aspect_ratio, 0.1f, 1000.0f)) {
    update_view_matrix();
}

void Camera::set_position(const glm::vec3& position) {
    this->position = position;
}

void Camera::set_pitch(const float pitch) {
    this->pitch = pitch;
}

void Camera::set_yaw(const float yaw) {
    this->yaw = yaw;
}

void Camera::set_rotation(const float rotation) {
    this->rotation_z = rotation;
}

void Camera::update_view_matrix() {
    const glm::mat4 translation = translate(glm::mat4(1.0f), this->position);
    const glm::mat4 rotation = rotate(glm::mat4(1.0f), glm::radians(this->rotation_z), glm::vec3(0, 0, 1));
    const auto orientation = glm::quat(radians(glm::vec3(this->pitch, -this->yaw, 0.0f)));
    const glm::mat4 transform = translation * toMat4(orientation) * rotation;

    this->view_matrix = inverse(transform);
    this->view_projection_matrix = this->projection_matrix * this->view_matrix;
}
