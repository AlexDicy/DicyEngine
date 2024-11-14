#pragma once
#include "math/Rotation.h"

class Transform {
public:
    Transform(const glm::vec3 position, const Rotation rotation, const glm::vec3 scale) : position(position), rotation(rotation), scale(scale) {
        this->invalidate();
    }
    Transform(const glm::vec3 position, const Rotation rotation) : Transform(position, rotation, glm::vec3(1.0f)) {}
    Transform(const glm::vec3 position) : Transform(position, Rotation(), glm::vec3(1.0f)) {}
    Transform() : position(glm::vec3(0.0f)), rotation(Rotation()), scale(glm::vec3(1.0f)) {}

    glm::mat4& getAsMatrix() {
        return this->getGlobalTransformMatrix();
    }

    void setPosition(const glm::vec3& position) {
        this->position = position;
        this->invalidate();
    }

    void setRotation(const Rotation& rotation) {
        this->rotation = rotation;
        this->invalidate();
    }

    void setScale(const glm::vec3& scale) {
        this->scale = scale;
        this->invalidate();
    }

private:
    glm::mat4& getLocalTransformMatrix() {
        if (this->recalculateLocal) {
            this->recalculateLocal = false;
            const glm::mat4 translationMat = glm::translate(glm::mat4(1.0f), this->position);
            const glm::mat4 rotationMat = glm::toMat4(this->rotation.toQuaternion());
            const glm::mat4 scaleMat = glm::scale(translationMat * rotationMat, this->scale);
            this->localTransformMatrix = scaleMat;
        }
        return this->localTransformMatrix;
    }

    glm::mat4& getGlobalTransformMatrix() {
        if (this->recalculateGlobal) {
            this->recalculateGlobal = false;
            if (this->parent) {
                this->globalTransformMatrix = this->parent->getAsMatrix() * this->getLocalTransformMatrix();
            } else {
                this->globalTransformMatrix = this->getLocalTransformMatrix();
            }
        }
        return this->globalTransformMatrix;
    }

    void invalidate() {
        this->recalculateLocal = true;
        this->recalculateGlobal = true;
    }

    // local position
    glm::vec3 position;
    Rotation rotation;
    glm::vec3 scale;

    glm::mat4 localTransformMatrix = glm::mat4(1.0f);
    glm::mat4 globalTransformMatrix = glm::mat4(1.0f);
    bool recalculateLocal = false;
    bool recalculateGlobal = false;

    Transform* parent = nullptr;
};
