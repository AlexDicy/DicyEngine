#include "pch/enginepch.h"
#include "JoltPhysics.h"

#include "JoltPhysicsBody.h"
#include "physics/shapes/BoxShape.h"
#include "physics/shapes/SphereShape.h"

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

    bodyInterface = &physicsSystem.GetBodyInterface();
    // todo: use the following code

    // bodyInterface.RemoveBody(sphereId);
    //
    //// Destroy the sphere. After this the sphere ID is no longer valid.
    // bodyInterface.DestroyBody(sphereId);
    //
    //// Remove and destroy the floor
    // bodyInterface.RemoveBody(floor->GetID());
    // bodyInterface.DestroyBody(floor->GetID());
}

void JoltPhysics::update(const float deltaTime, const int steps) {
    DE_PROFILE_FUNCTION();
    static JPH::uint step = 0;
    ++step;

    physicsSystem.Update(deltaTime, steps, tempAllocator, jobSystem);


    // JPH::RVec3 position = bodyInterface.GetCenterOfMassPosition(sphereId);
    // JPH::Vec3 velocity = bodyInterface.GetLinearVelocity(sphereId);
    // std::cout << "Step " << step << ": Position = (" << position.GetX() << ", " << position.GetY() << ", " << position.GetZ() << "), Velocity = (" << velocity.GetX() << ", "
    // << velocity.GetY() << ", " << velocity.GetZ() << ")" << std::endl;
}

Ref<PhysicsBody> JoltPhysics::createBody(const PhysicsShape* shape, Transform& transform, const PhysicsLayer& layer) {
    const JPH::RVec3 position(transform.getPosition().x, transform.getPosition().y, transform.getPosition().z);
    const glm::quat rotation = transform.getRotation().getQuaternion();
    const JPH::Quat joltRotation(rotation.x, rotation.y, rotation.z, rotation.w);
    const JPH::Shape* joltShape = createJoltShape(shape);
    const JPH::BodyCreationSettings sphereSettings(joltShape, position, joltRotation, JoltLayers::getJoltMotionType(layer), JoltLayers::getJoltLayer(layer));
    JPH::BodyID sphereId = bodyInterface->CreateBody(sphereSettings)->GetID();
    return std::make_shared<JoltPhysicsBody>(sphereId);
}

void JoltPhysics::addBody(const Ref<PhysicsBody>& body) {
    const Ref<JoltPhysicsBody>& joltBody = std::static_pointer_cast<JoltPhysicsBody>(body);
    bodyInterface->AddBody(joltBody->getId(), JPH::EActivation::Activate);
}

void JoltPhysics::syncTransform(const Ref<PhysicsBody>& body, Transform& transform) {
    const Ref<JoltPhysicsBody>& joltBody = std::static_pointer_cast<JoltPhysicsBody>(body);
    JPH::RVec3 position;
    JPH::Quat rotation;
    bodyInterface->GetPositionAndRotation(joltBody->getId(), position, rotation);
    transform.setPosition(glm::vec3(position.GetX(), position.GetY(), position.GetZ()));
    transform.setRotation(Rotation(glm::quat(rotation.GetX(), rotation.GetY(), rotation.GetZ(), rotation.GetW())));
}

JPH::Shape* JoltPhysics::createJoltShape(const PhysicsShape* shape) {
    switch (shape->getType()) {
        case ShapeType::SPHERE:
            {
                const auto sphereShape = dynamic_cast<const SphereShape*>(shape);
                return new JPH::SphereShape(sphereShape->getRadius());
            }
        case ShapeType::BOX:
            {
                const auto boxShape = dynamic_cast<const BoxShape*>(shape);
                const glm::vec3& dimensions = boxShape->getDimensions();
                return new JPH::BoxShape(JPH::Vec3(dimensions.x * 0.5f, dimensions.y * 0.5f, dimensions.z * 0.5f));
            }
    }
    DE_ASSERT(false, "Unsupported shape type")
    return nullptr;
}
