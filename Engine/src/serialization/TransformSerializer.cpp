#include "pch/enginepch.h"
#include "TransformSerializer.h"

void TransformSerializer::serialize(Transform& transform, toml::table& out) {
    out.emplace("position", toml::array{transform.getPosition().x, transform.getPosition().y, transform.getPosition().z});
    // TODO: create a RotationSerializer
    // TODO: euler angles or quaternions?
    out.emplace("rotation", toml::array{transform.getRotation().getPitch(), transform.getRotation().getYaw(), transform.getRotation().getRoll()});
    out.emplace("scale", toml::array{transform.getScale().x, transform.getScale().y, transform.getScale().z});
}
