﻿#include "pch/enginepch.h"
#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

Camera::Camera(const glm::mat4& projection_matrix) : projection_matrix(projection_matrix) {
    update_view_matrix();
}

void Camera::set_position(const glm::vec3& position) {
    this->position = position;
}

void Camera::set_rotation(const Rotation& rotation) {
    this->rotation = rotation;
}

void Camera::update_view_matrix() {
    const glm::mat4 translation = translate(glm::mat4(1.0f), this->position);
    const auto orientation = this->rotation.to_quaternion();
    const glm::mat4 transform = translation * toMat4(orientation);

    this->view_matrix = inverse(transform);
    this->view_projection_matrix = this->projection_matrix * this->view_matrix;
}
