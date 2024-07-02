#include "pch/enginepch.h"
#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

Camera::Camera(const float left, const float right, const float bottom, const float top) : projection_matrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)) {
    update_view_matrix();
}

Camera::Camera(const float fov_y, const float aspect_ratio) : projection_matrix(glm::perspective(glm::radians(fov_y), aspect_ratio, 0.1f, 1000.0f)) {
    update_view_matrix();
}

void Camera::set_position(const glm::vec3& position) {
    this->position = position;
    update_view_matrix();
}

void Camera::set_orientation(const float pitch, const float yaw) {
    this->pitch = pitch;
    this->yaw = yaw;
    update_view_matrix();
}

void Camera::set_rotation(const float rotation) {
    this->rotation_z = rotation;
    update_view_matrix();
}

void Camera::update_view_matrix() {
    const glm::mat4 translation = translate(glm::mat4(1.0f), this->position);
    // const glm::mat4 rotation = rotate(glm::mat4(1.0f), glm::radians(this->rotation_z), glm::vec3(0, 0, 1)); for ortho
    const auto orientation = glm::quat(glm::vec3(this->pitch, -this->yaw, 0.0f));
    const glm::mat4 transform = translation * glm::toMat4(orientation);

    this->view_matrix = inverse(transform);
    this->view_projection_matrix = this->projection_matrix * this->view_matrix;
}
