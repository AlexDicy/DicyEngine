#include "pch/enginepch.h"
#include "JoltLayers.h"

JPH::BroadPhaseLayer JoltBroadPhaseLayers::getJoltBroadPhaseLayer(const Physics::Layer layer) {
    switch (layer) {
        case Physics::Layer::NON_MOVING:
            return NON_MOVING;
        case Physics::Layer::MOVING:
            return MOVING;
        default:
            DE_ASSERT(false, "Invalid PhysicsLayer")
            return NON_MOVING;
    }
}

Physics::Layer JoltBroadPhaseLayers::getPhysicsLayer(const JPH::BroadPhaseLayer layer) {
    switch (static_cast<JPH::BroadPhaseLayer::Type>(layer)) {
        case static_cast<JPH::BroadPhaseLayer::Type>(NON_MOVING):
            return Physics::Layer::NON_MOVING;
        case static_cast<JPH::BroadPhaseLayer::Type>(MOVING):
            return Physics::Layer::MOVING;
        default:
            DE_ASSERT(false, "Invalid Jolt BroadPhaseLayer")
            return Physics::Layer::NON_MOVING;
    }
}
