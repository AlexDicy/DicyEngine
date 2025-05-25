#pragma once

struct PointLight {
    glm::vec3 position;
    glm::vec3 color;
    float intensity;
    float farPlane = 20.0f;
};
