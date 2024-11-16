#pragma once
#include "math/Rotation.h"


class Entity;

class Transform {
    friend class Entity;

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

    glm::vec3 getPosition() const {
        return this->position;
    }

    Rotation getRotation() const {
        return this->rotation;
    }

    glm::vec3 getScale() const {
        return this->scale;
    }

protected:
    void setOwner(Entity* owner) {
        this->owner = owner;
        this->invalidateGlobal();
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

    glm::mat4& getGlobalTransformMatrix();

    void invalidate();
    void invalidateLocal();
    void invalidateGlobal();

    // local space
    glm::vec3 position;
    Rotation rotation;
    glm::vec3 scale;

    glm::mat4 localTransformMatrix = glm::mat4(1.0f);
    glm::mat4 globalTransformMatrix = glm::mat4(1.0f);
    bool recalculateLocal = false;
    bool recalculateGlobal = false;

    Entity* owner = nullptr;
};
