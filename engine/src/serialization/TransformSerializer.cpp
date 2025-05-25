#include "pch/enginepch.h"
#include "TransformSerializer.h"

#include "RotationSerializer.h"

void TransformSerializer::serialize(Transform& transform, toml::table& out) {
    out.emplace("position", toml::array{transform.getPosition().x, transform.getPosition().y, transform.getPosition().z});
    RotationSerializer rotationSerializer;
    rotationSerializer.serialize(transform.getRotation(), out);
    out.emplace("scale", toml::array{transform.getScale().x, transform.getScale().y, transform.getScale().z});
}
