#pragma once
#include "camera.h"

class PerspectiveCamera : public Camera {
public:
    PerspectiveCamera(float fov_y, float aspect_ratio);

    void set_aspect_ratio(float aspect_ratio) override;

    static glm::mat4 calculate_projection_matrix(float fov_y, float aspect_ratio);

private:
    float fov_y;
};
