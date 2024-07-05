#pragma once
#include "camera.h"

class OrthographicCamera : public Camera {
public:
    OrthographicCamera(const float left, const float right, const float bottom, const float top) : Camera(glm::ortho(left, right, bottom, top, 0.1f, 1000.0f)) {}

    void set_aspect_ratio(float aspect_ratio) override {}
};
