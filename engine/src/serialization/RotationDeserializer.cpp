#include "pch/enginepch.h"
#include "RotationDeserializer.h"

Rotation RotationDeserializer::deserialize(const toml::table& in) {
    const toml::array& rotationArray = *in["rotation"].as_array();
    const float x = rotationArray[0].value<double>().value();
    const float y = rotationArray[1].value<double>().value();
    const float z = rotationArray[2].value<double>().value();
    const float w = rotationArray[3].value<double>().value();
    return {glm::quat(w, x, y, z)};
}
