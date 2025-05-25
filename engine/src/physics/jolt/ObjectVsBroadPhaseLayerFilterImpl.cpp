#include "pch/enginepch.h"
#include "ObjectVsBroadPhaseLayerFilterImpl.h"

bool ObjectVsBroadPhaseLayerFilterImpl::ShouldCollide(const JPH::ObjectLayer layer1, const JPH::BroadPhaseLayer layer2) const {
    switch (layer1) {
        case JoltLayers::NON_MOVING:
            return layer2 == JoltBroadPhaseLayers::MOVING;
        case JoltLayers::MOVING:
            return true;
        default:
            JPH_ASSERT(false);
            return false;
    }
}
