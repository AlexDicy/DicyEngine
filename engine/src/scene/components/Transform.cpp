#include "pch/enginepch.h"
#include "Transform.h"
#include "scene/entities/Entity.h"

#include <glm/gtx/matrix_decompose.hpp>


glm::mat4& Transform::getGlobalTransformMatrix() {
    if (this->recalculateGlobal || this->rotation.needsRecalculation()) {
        this->recalculateGlobal = false;
        if (this->owner && this->owner->hasParent()) {
            const Ref<Entity>& parent = this->owner->getParent();
            this->globalTransformMatrix = parent->getTransform()->getAsMatrix() * this->getLocalTransformMatrix();
        } else {
            this->globalTransformMatrix = this->getLocalTransformMatrix();
        }
    }
    return this->globalTransformMatrix;
}

void Transform::setFromMatrix(const glm::mat4& matrix) {
    glm::vec3 scale;
    glm::quat rotation;
    glm::vec3 translation;
    glm::vec3 skew;
    glm::vec4 perspective;
    glm::decompose(matrix, scale, rotation, translation, skew, perspective);

    this->position = translation;
    this->rotation = rotation;
    this->scale = scale;
    this->invalidate();
}

void Transform::multiplyByMatrix(const glm::mat4& matrix) {
    this->setFromMatrix(this->getLocalTransformMatrix() * matrix);
}

void Transform::invalidate() {
    this->invalidateLocal();
    this->invalidateGlobal();
}

void Transform::invalidateLocal() {
    this->recalculateLocal = true;
}

void Transform::invalidateGlobal() {
    this->recalculateGlobal = true;
    if (this->owner) {
        // invalidate children
        for (const Ref<Entity>& child : this->owner->getChildren()) {
            child->getTransform()->invalidateGlobal();
        }
    }
}
