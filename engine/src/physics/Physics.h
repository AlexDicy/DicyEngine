#pragma once
#include "PhysicsBody.h"
#include "PhysicsShape.h"

class Physics {
public:
    virtual ~Physics() = default;

    enum class Layer : unsigned int { NON_MOVING = 0, MOVING = 1, NUM_LAYERS = 2 };

    virtual void init() = 0;
    virtual void update(float deltaTime, int steps) = 0;

    virtual Ref<PhysicsBody> createBody(const PhysicsShape* shape, Transform& transform, const Layer& layer) = 0;
    virtual void addBody(const Ref<PhysicsBody>& body) = 0;

    virtual void syncTransform(const Ref<PhysicsBody>& body, Transform& transform) = 0;
};
