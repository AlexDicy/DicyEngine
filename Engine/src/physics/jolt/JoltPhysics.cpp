#include "pch/enginepch.h"
#include "JoltPhysics.h"

#include <Jolt/Jolt.h>
#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>

void JoltPhysics::init() {
    JPH::RegisterDefaultAllocator();
    JPH::Factory::sInstance = new JPH::Factory();
    JPH::RegisterTypes();

    this->tempAllocator = new JPH::TempAllocatorImpl(static_cast<size_t>(10) * 1024 * 1024); // 10 MB
    this->jobSystem = new JPH::JobSystemThreadPool(JPH::cMaxPhysicsJobs, JPH::cMaxPhysicsBarriers, static_cast<int>(std::thread::hardware_concurrency()) - 1);

    constexpr JPH::uint maxBodies = 65536;
    constexpr JPH::uint numBodyMutexes = 0; // use default
    constexpr JPH::uint maxBodyPairs = 65536;
    constexpr JPH::uint maxContactConstraints = 10240;
    physicsSystem.Init(maxBodies, numBodyMutexes, maxBodyPairs, maxContactConstraints, broadPhaseLayerInterface, objectVsBroadphaseLayerFilter, objectVsObjectLayerFilter);

    // todo: probably remove test code
    JPH::BodyInterface& bodyInterface = physicsSystem.GetBodyInterface();
    JPH::BoxShapeSettings floorShapeSettings(JPH::Vec3(100.0f, 1.0f, 100.0f));
    floorShapeSettings
        .SetEmbedded(); // A ref counted object on the stack (base class RefTarget) should be marked as such to prevent it from being freed when its reference count goes to 0.
    JPH::ShapeSettings::ShapeResult floorShapeResult = floorShapeSettings.Create();
    JPH::ShapeRefC floorShape = floorShapeResult.Get();
    JPH::BodyCreationSettings floorSettings(floorShape, JPH::RVec3(0.0, -1.0, 0.0), JPH::Quat::sIdentity(), JPH::EMotionType::Static, Layers::NON_MOVING);
    JPH::Body* floor = bodyInterface.CreateBody(floorSettings);
    bodyInterface.AddBody(floor->GetID(), JPH::EActivation::DontActivate);
    JPH::BodyCreationSettings sphereSettings(new JPH::SphereShape(0.5f), JPH::RVec3(0.0, 2.0, 0.0), JPH::Quat::sIdentity(), JPH::EMotionType::Dynamic, Layers::MOVING);
    JPH::BodyID sphereId = bodyInterface.CreateAndAddBody(sphereSettings, JPH::EActivation::Activate);
    bodyInterface.SetLinearVelocity(sphereId, JPH::Vec3(0.0f, -5.0f, 0.0f));
    const float cDeltaTime = 1.0f / 60.0f;
    physicsSystem.OptimizeBroadPhase();

    JPH::uint step = 0;
    while (bodyInterface.IsActive(sphereId)) {
        // Next step
        ++step;

        // Output current position and velocity of the sphere
        JPH::RVec3 position = bodyInterface.GetCenterOfMassPosition(sphereId);
        JPH::Vec3 velocity = bodyInterface.GetLinearVelocity(sphereId);
        std::cout << "Step " << step << ": Position = (" << position.GetX() << ", " << position.GetY() << ", " << position.GetZ() << "), Velocity = (" << velocity.GetX() << ", "
                  << velocity.GetY() << ", " << velocity.GetZ() << ")" << std::endl;

        // If you take larger steps than 1 / 60th of a second you need to do multiple collision steps in order to keep the simulation stable. Do 1 collision step per 1 / 60th of a
        // second (round up).
        const int cCollisionSteps = 1;

        // Step the world
        physicsSystem.Update(cDeltaTime, cCollisionSteps, tempAllocator, jobSystem);
    }

    bodyInterface.RemoveBody(sphereId);

    // Destroy the sphere. After this the sphere ID is no longer valid.
    bodyInterface.DestroyBody(sphereId);

    // Remove and destroy the floor
    bodyInterface.RemoveBody(floor->GetID());
    bodyInterface.DestroyBody(floor->GetID());

    // Unregisters all types with the factory and cleans up the default material
    JPH::UnregisterTypes();

    // Destroy the factory
    delete JPH::Factory::sInstance;
    JPH::Factory::sInstance = nullptr;
}

void JoltPhysics::update(const float deltaTime, const int steps) {
    physicsSystem.Update(deltaTime, steps, tempAllocator, jobSystem);
}
