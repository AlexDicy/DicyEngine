#include "pch/enginepch.h"
#include "BPLayerInterfaceImpl.h"

BPLayerInterfaceImpl::BPLayerInterfaceImpl() {
    objectToBroadPhase[static_cast<unsigned int>(PhysicsLayer::NON_MOVING)] = JoltBroadPhaseLayers::NON_MOVING;
    objectToBroadPhase[static_cast<unsigned int>(PhysicsLayer::MOVING)] = JoltBroadPhaseLayers::MOVING;
}

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
const char* BPLayerInterfaceImpl::GetBroadPhaseLayerName(const JPH::BroadPhaseLayer inLayer) const {
    switch (static_cast<JPH::BroadPhaseLayer::Type>(inLayer)) {
        case static_cast<JPH::BroadPhaseLayer::Type>(JoltBroadPhaseLayers::NON_MOVING):
            return "NON_MOVING";
        case static_cast<JPH::BroadPhaseLayer::Type>(JoltBroadPhaseLayers::MOVING):
            return "MOVING";
        default:
            JPH_ASSERT(false);
            return "INVALID";
    }
}
#endif
