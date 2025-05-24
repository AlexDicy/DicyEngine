#pragma once
#include "Layers.h"

/// class that determines if two object layers can collide
class ObjectLayerPairFilterImpl : public JPH::ObjectLayerPairFilter {
public:
    bool ShouldCollide(JPH::ObjectLayer object1, JPH::ObjectLayer object2) const override;
};
