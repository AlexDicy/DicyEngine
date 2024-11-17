#pragma once

#include "math/Rotation.h"
#include <glm/glm.hpp>


class Camera {
public:
    virtual ~Camera() = default;
    explicit Camera(const glm::mat4& projectionMatrix);

    const glm::vec3& getPosition() const {
        return this->position;
    }

    float getPitch() const {
        return this->rotation.getPitch();
    }

    float getYaw() const {
        return this->rotation.getYaw();
    }

    const Rotation& getRotation() const {
        return this->rotation;
    }

    const glm::mat4& getViewProjectionMatrix(const bool recalculate) {
        if (recalculate) {
            this->updateViewMatrix();
        }
        return this->viewProjectionMatrix;
    }

    const glm::mat4& getViewMatrix() const {
        return this->viewMatrix;
    }

    const glm::mat4& getProjectionMatrix() const {
        return this->projectionMatrix;
    }

    void setPosition(const glm::vec3& position);
    void setRotation(const Rotation& rotation);
    void updateViewMatrix();

    virtual void setAspectRatio(float aspectRatio);

protected:
    glm::vec3 position = glm::vec3(0.0f);
    Rotation rotation;

    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    glm::mat4 viewProjectionMatrix;
};
