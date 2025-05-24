#include "pch/enginepch.h"
#include "BPLayerInterfaceImpl.h"

BPLayerInterfaceImpl::BPLayerInterfaceImpl() {
    // create a mapping table from object to broad phase layer
    objectToBroadPhase[Layers::NON_MOVING] = BroadPhaseLayers::NON_MOVING;
    objectToBroadPhase[Layers::MOVING] = BroadPhaseLayers::MOVING;
}

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
const char* BPLayerInterfaceImpl::GetBroadPhaseLayerName(const JPH::BroadPhaseLayer inLayer) const {
    switch (static_cast<JPH::BroadPhaseLayer::Type>(inLayer)) {
        case static_cast<JPH::BroadPhaseLayer::Type>(BroadPhaseLayers::NON_MOVING):
            return "NON_MOVING";
        case static_cast<JPH::BroadPhaseLayer::Type>(BroadPhaseLayers::MOVING):
            return "MOVING";
        default:
            JPH_ASSERT(false);
            return "INVALID";
    }
}
#endif
