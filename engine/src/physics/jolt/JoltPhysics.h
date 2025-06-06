#pragma once
#include "physics/Physics.h"

#include "BPLayerInterfaceImpl.h"
#include "ObjectLayerPairFilterImpl.h"
#include "ObjectVsBroadPhaseLayerFilterImpl.h"

#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSystem.h>

class JoltPhysics : public Physics {
public:
    void init() override;
    void update(float deltaTime, int steps) override;

    Ref<PhysicsBody> createBody(const PhysicsShape* shape, Transform& transform, const Layer& layer) override;
    void addBody(const Ref<PhysicsBody>& body) override;

    void syncTransform(const Ref<PhysicsBody>& body, Transform& transform) override;

private:
    static JPH::Shape* createJoltShape(const PhysicsShape* shape);

    BPLayerInterfaceImpl broadPhaseLayerInterface;
    ObjectVsBroadPhaseLayerFilterImpl objectVsBroadphaseLayerFilter;
    ObjectLayerPairFilterImpl objectVsObjectLayerFilter;

    JPH::PhysicsSystem physicsSystem;
    JPH::BodyInterface* bodyInterface = nullptr;

    JPH::TempAllocatorImpl* tempAllocator = nullptr;
    JPH::JobSystemThreadPool* jobSystem = nullptr;
};
