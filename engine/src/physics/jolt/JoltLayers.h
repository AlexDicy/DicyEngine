#pragma once
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/MotionType.h>
#include <Jolt/Physics/Collision/ObjectLayer.h>
#include <Jolt/Physics/Collision/BroadPhase/BroadPhaseLayer.h>

namespace JoltLayers {
    static constexpr JPH::ObjectLayer NON_MOVING = 0;
    static constexpr JPH::ObjectLayer MOVING = 1;
    static constexpr JPH::ObjectLayer NUM_LAYERS = 2;

    static constexpr JPH::ObjectLayer getJoltLayer(const Physics::Layer& layer) {
        return static_cast<JPH::ObjectLayer>(layer);
    }

    static constexpr Physics::Layer getPhysicsLayer(const JPH::ObjectLayer& layer) {
        return static_cast<Physics::Layer>(layer);
    }

    static constexpr JPH::EMotionType getJoltMotionType(const Physics::Layer& layer) {
        return layer == Physics::Layer::MOVING ? JPH::EMotionType::Dynamic : JPH::EMotionType::Static;
    }
}

namespace JoltBroadPhaseLayers {
    static constexpr JPH::BroadPhaseLayer NON_MOVING(0);
    static constexpr JPH::BroadPhaseLayer MOVING(1);
    static constexpr JPH::uint NUM_LAYERS(2);

    static JPH::BroadPhaseLayer getJoltBroadPhaseLayer(Physics::Layer layer);
    static Physics::Layer getPhysicsLayer(JPH::BroadPhaseLayer layer);
};
