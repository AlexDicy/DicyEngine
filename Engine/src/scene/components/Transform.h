#pragma once
#include "math/Rotation.h"

class Transform {
public:
    Transform(const glm::vec3 position, const Rotation rotation, const glm::vec3 scale) : position(position), rotation(rotation), scale(scale) {}
    Transform(const glm::vec3 position, const Rotation rotation) : Transform(position, rotation, glm::vec3(1.0f)) {}
    Transform(const glm::vec3 position) : Transform(position, Rotation(), glm::vec3(1.0f)) {}
    Transform() : Transform(glm::vec3(0.0f)) {}

    glm::mat4 getAsMatrix() const {
        const glm::mat4 translationMat = glm::translate(glm::mat4(1.0f), this->position);
        const glm::mat4 rotationMat = glm::toMat4(this->rotation.toQuaternion());
        const glm::mat4 scaleMat = glm::scale(translationMat * rotationMat, this->scale);
        return scaleMat;
    }

    void setPosition(const glm::vec3& position) {
        this->position = position;
    }

    void setRotation(const Rotation& rotation) {
        this->rotation = rotation;
    }

    void setScale(const glm::vec3& scale) {
        this->scale = scale;
    }

private:
    glm::vec3 position;
    Rotation rotation;
    glm::vec3 scale;
};
