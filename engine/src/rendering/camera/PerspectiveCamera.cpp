#include "pch/enginepch.h"
#include "PerspectiveCamera.h"

PerspectiveCamera::PerspectiveCamera(const float fovY, const float aspectRatio) : Camera(calculateProjectionMatrix(fovY, aspectRatio)), fovY(fovY) {}

void PerspectiveCamera::setAspectRatio(const float aspectRatio) {
    this->projectionMatrix = calculateProjectionMatrix(this->fovY, aspectRatio);
    updateViewMatrix();
}

glm::mat4 PerspectiveCamera::calculateProjectionMatrix(const float fovY, const float aspectRatio) {
    return glm::perspective(glm::radians(fovY), aspectRatio, 0.1f, 1000.0f);
}
