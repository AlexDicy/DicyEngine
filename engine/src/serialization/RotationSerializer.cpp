#include "pch/enginepch.h"
#include "RotationSerializer.h"

void RotationSerializer::serialize(Rotation& rotation, toml::table& out) {
    const glm::quat& quaternion = rotation.getQuaternion();
    out.emplace("rotation", toml::array{quaternion.x, quaternion.y, quaternion.z, quaternion.w});
}
