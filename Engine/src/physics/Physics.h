#pragma once
#include "PhysicsBody.h"
#include "PhysicsLayer.h"
#include "PhysicsShape.h"

class Physics {
public:
    virtual ~Physics() = default;

    virtual void init() = 0;
    virtual void update(float deltaTime, int steps) = 0;

    virtual Ref<PhysicsBody> createBody(const PhysicsShape* shape, Transform& transform, const PhysicsLayer& layer) = 0;
    virtual void addBody(const Ref<PhysicsBody>& body) = 0;

    virtual void syncTransform(const Ref<PhysicsBody>& body, Transform& transform) = 0;
};
