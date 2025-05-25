#pragma once
#include "math/Rotation.h"


class Entity;

class Transform {
    friend class Entity;
    friend class TransformSerializer;
    friend class JavaBindings;

public:
    Transform(const glm::vec3 position, Rotation rotation, const glm::vec3 scale) : position(position), rotation(std::move(rotation)), scale(scale) {
        this->invalidate();
    }
    Transform(const glm::vec3 position, Rotation rotation) : Transform(position, std::move(rotation), glm::vec3(1.0f)) {}
    Transform(const glm::vec3 position) : Transform(position, Rotation(), glm::vec3(1.0f)) {}
    Transform() : position(glm::vec3(0.0f)), rotation(Rotation()), scale(glm::vec3(1.0f)) {}

    glm::vec3 getPosition() const {
        return this->position;
    }

    Rotation& getRotation() {
        return this->rotation;
    }

    glm::vec3 getScale() const {
        return this->scale;
    }

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

    void setFromMatrix(const glm::mat4& matrix);
    void multiplyByMatrix(const glm::mat4& matrix);

protected:
    void setOwner(Entity* owner) {
        this->owner = owner;
        this->invalidateGlobal();
    }

private:
    glm::mat4& getLocalTransformMatrix() {
        if (this->recalculateLocal || this->rotation.needsRecalculation()) {
            this->recalculateLocal = false;
            const glm::mat4 translationMat = glm::translate(glm::mat4(1.0f), this->position);
            const glm::mat4 rotationMat = glm::toMat4(this->rotation.getQuaternion());
            const glm::mat4 scaleMat = glm::scale(translationMat * rotationMat, this->scale);
            this->localTransformMatrix = scaleMat;
        }
        return this->localTransformMatrix;
    }

    glm::mat4& getGlobalTransformMatrix();

protected:
    void invalidate();
    void invalidateLocal();
    void invalidateGlobal();

    //
    // Keep in sync with scripting/com.dicydev.engine.components.Transform
    //

    // local space
    glm::vec3 position;
    Rotation rotation;
    glm::vec3 scale;

private:
    glm::mat4 localTransformMatrix = glm::mat4(1.0f);
    glm::mat4 globalTransformMatrix = glm::mat4(1.0f);
    bool recalculateLocal = false;
    bool recalculateGlobal = false;

    Entity* owner = nullptr;
};
