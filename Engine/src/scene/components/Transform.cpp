#include "pch/enginepch.h"
#include "Transform.h"
#include "scene/entities/Entity.h"

glm::mat4& Transform::getGlobalTransformMatrix() {
    if (this->recalculateGlobal) {
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

void Transform::invalidate() {
    this->invalidateLocal();
    this->invalidateGlobal();
}

void Transform::invalidateLocal() {
    this->recalculateLocal = true;
}

void Transform::invalidateGlobal() {
    this->recalculateGlobal = true;
    // invalidate children
    for (const Ref<Entity>& child : this->owner->getChildren()) {
        child->getTransform()->invalidateGlobal();
    }
}
