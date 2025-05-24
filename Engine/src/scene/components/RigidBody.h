#pragma once
#include "physics/Physics.h"
#include "physics/PhysicsBody.h"
#include "physics/PhysicsLayer.h"
#include "physics/PhysicsShape.h"

struct RigidBody {
    Ref<PhysicsBody> physicsBody;

    RigidBody(const Ref<Physics>& physics, const PhysicsShape& shape, const Transform& transform, const PhysicsLayer layer) :
        physicsBody(physics->createBody(shape, transform, layer)) {}
};
