#pragma once
#include "JoltLayers.h"

#include <Jolt/Physics/Collision/BroadPhase/BroadPhaseLayer.h>

/// class that determines if an object layer can collide with a broadphase layer
class ObjectVsBroadPhaseLayerFilterImpl final : public JPH::ObjectVsBroadPhaseLayerFilter {
public:
    bool ShouldCollide(JPH::ObjectLayer layer1, JPH::BroadPhaseLayer layer2) const override;
};
