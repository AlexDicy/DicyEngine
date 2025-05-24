#pragma once
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/ObjectLayer.h>
#include <Jolt/Physics/Collision/BroadPhase/BroadPhaseLayer.h>

namespace JoltLayers {
    static constexpr JPH::ObjectLayer NON_MOVING = 0;
    static constexpr JPH::ObjectLayer MOVING = 1;
    static constexpr JPH::ObjectLayer NUM_LAYERS = 2;

    static constexpr JPH::ObjectLayer getJoltLayer(PhysicsLayer layer) {
        return static_cast<JPH::ObjectLayer>(layer);
    }

    static constexpr PhysicsLayer getPhysicsLayer(JPH::ObjectLayer layer) {
        return static_cast<PhysicsLayer>(layer);
    }
}

namespace JoltBroadPhaseLayers {
    static constexpr JPH::BroadPhaseLayer NON_MOVING(0);
    static constexpr JPH::BroadPhaseLayer MOVING(1);
    static constexpr JPH::uint NUM_LAYERS(2);

    static JPH::BroadPhaseLayer getJoltBroadPhaseLayer(PhysicsLayer layer);
    static PhysicsLayer getPhysicsLayer(JPH::BroadPhaseLayer layer);
};
