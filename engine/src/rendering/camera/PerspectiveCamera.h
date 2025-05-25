#pragma once
#include "Camera.h"

class PerspectiveCamera : public Camera {
public:
    PerspectiveCamera(float fovY, float aspectRatio);

    void setAspectRatio(float aspectRatio) override;

    static glm::mat4 calculateProjectionMatrix(float fovY, float aspectRatio);

private:
    float fovY;
};
