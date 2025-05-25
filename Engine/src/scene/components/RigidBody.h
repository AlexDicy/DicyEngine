#pragma once
#include "physics/Physics.h"
#include "physics/PhysicsBody.h"
#include "physics/PhysicsLayer.h"
#include "physics/PhysicsShape.h"

class RigidBody {
public:
    RigidBody(const PhysicsShape& shape, const PhysicsLayer& layer) : shape(shape), layer(layer) {}

    void initializeBody(const Ref<Physics>& physics, Transform& transform) {
        if (this->isInitialized()) {
            return;
        }
        this->physicsBody = physics->createBody(shape, transform, layer);
    }

    bool isInitialized() const {
        return physicsBody != nullptr;
    }

    const Ref<PhysicsBody>& getPhysicsBody() const {
        return physicsBody;
    }

private:
    PhysicsShape shape;
    PhysicsLayer layer;

    Ref<PhysicsBody> physicsBody = nullptr;
};
