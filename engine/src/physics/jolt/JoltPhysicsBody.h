#pragma once
#include "physics/PhysicsBody.h"

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/BodyID.h>

class JoltPhysicsBody : public PhysicsBody {
public:
    explicit JoltPhysicsBody(const JPH::BodyID& bodyId) : bodyId(bodyId) {}

    JPH::BodyID getId() const {
        return bodyId;
    }

private:
    JPH::BodyID bodyId;
};
