#pragma once
#include "physics/Physics.h"

#include "BPLayerInterfaceImpl.h"
#include "ObjectLayerPairFilterImpl.h"
#include "ObjectVsBroadPhaseLayerFilterImpl.h"

class JoltPhysics : public Physics {
public:
    void init() override;

private:
    BPLayerInterfaceImpl broadPhaseLayerInterface;
    ObjectVsBroadPhaseLayerFilterImpl objectVsBroadphaseLayerFilter;
    ObjectLayerPairFilterImpl objectVsObjectLayerFilter;
};
