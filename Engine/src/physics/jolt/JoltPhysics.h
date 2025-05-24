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

private:
    BPLayerInterfaceImpl broadPhaseLayerInterface;
    ObjectVsBroadPhaseLayerFilterImpl objectVsBroadphaseLayerFilter;
    ObjectLayerPairFilterImpl objectVsObjectLayerFilter;

    JPH::PhysicsSystem physicsSystem;

    JPH::TempAllocatorImpl* tempAllocator = nullptr;
    JPH::JobSystemThreadPool* jobSystem = nullptr;
};
