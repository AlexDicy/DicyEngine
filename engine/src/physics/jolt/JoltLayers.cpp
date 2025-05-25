#include "pch/enginepch.h"
#include "JoltLayers.h"

JPH::BroadPhaseLayer JoltBroadPhaseLayers::getJoltBroadPhaseLayer(const PhysicsLayer layer) {
    switch (layer) {
        case PhysicsLayer::NON_MOVING:
            return NON_MOVING;
        case PhysicsLayer::MOVING:
            return MOVING;
        default:
            DE_ASSERT(false, "Invalid PhysicsLayer")
            return NON_MOVING;
    }
}

PhysicsLayer JoltBroadPhaseLayers::getPhysicsLayer(const JPH::BroadPhaseLayer layer) {
    switch (static_cast<JPH::BroadPhaseLayer::Type>(layer)) {
        case static_cast<JPH::BroadPhaseLayer::Type>(NON_MOVING):
            return PhysicsLayer::NON_MOVING;
        case static_cast<JPH::BroadPhaseLayer::Type>(MOVING):
            return PhysicsLayer::MOVING;
        default:
            DE_ASSERT(false, "Invalid Jolt BroadPhaseLayer")
            return PhysicsLayer::NON_MOVING;
    }
}
