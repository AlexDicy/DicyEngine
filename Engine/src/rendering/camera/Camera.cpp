#include "pch/enginepch.h"
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

Camera::Camera(const glm::mat4& projectionMatrix) : projectionMatrix(projectionMatrix) {
    updateViewMatrix();
}

void Camera::setPosition(const glm::vec3& position) {
    this->position = position;
}

void Camera::setRotation(const Rotation& rotation) {
    this->rotation = rotation;
}

void Camera::updateViewMatrix() {
    const glm::mat4 translation = translate(glm::mat4(1.0f), this->position);
    const auto orientation = this->rotation.toQuaternion();
    const glm::mat4 transform = translation * toMat4(orientation);

    this->viewMatrix = inverse(transform);
    this->viewProjectionMatrix = this->projectionMatrix * this->viewMatrix;
}
void Camera::setAspectRatio(float aspectRatio) {}
