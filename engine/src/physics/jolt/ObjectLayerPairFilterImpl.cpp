#include "pch/enginepch.h"
#include "ObjectLayerPairFilterImpl.h"

bool ObjectLayerPairFilterImpl::ShouldCollide(const JPH::ObjectLayer object1, const JPH::ObjectLayer object2) const {
    switch (object1) {
        case JoltLayers::NON_MOVING:
            return object2 == JoltLayers::MOVING; // non-moving only collides with moving
        case JoltLayers::MOVING:
            return true; // moving collides with everything
        default:
            JPH_ASSERT(false);
            return false;
    }
}
