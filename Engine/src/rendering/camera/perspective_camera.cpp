#include "pch/enginepch.h"
#include "perspective_camera.h"

PerspectiveCamera::PerspectiveCamera(const float fov_y, const float aspect_ratio) : Camera(calculate_projection_matrix(fov_y, aspect_ratio)), fov_y(fov_y) {}

void PerspectiveCamera::set_aspect_ratio(const float aspect_ratio) {
    this->projection_matrix = calculate_projection_matrix(this->fov_y, aspect_ratio);
    update_view_matrix();
}

glm::mat4 PerspectiveCamera::calculate_projection_matrix(const float fov_y, const float aspect_ratio) {
    return glm::perspective(glm::radians(fov_y), aspect_ratio, 0.1f, 1000.0f);
}
