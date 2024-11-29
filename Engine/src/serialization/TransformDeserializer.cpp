#include "pch/enginepch.h"
#include "TransformDeserializer.h"

#include "RotationDeserializer.h"

Transform TransformDeserializer::deserialize(const toml::table& in) {
    const toml::array& positionArray = *in["position"].as_array();
    const float posX = positionArray[0].value<double>().value();
    const float posY = positionArray[1].value<double>().value();
    const float posZ = positionArray[2].value<double>().value();
    const glm::vec3 position = {posX, posY, posZ};

    RotationDeserializer rotationDeserializer;
    const Rotation rotation = rotationDeserializer.deserialize(in);

    const toml::array& scaleArray = *in["scale"].as_array();
    const float scaleX = scaleArray[0].value<double>().value();
    const float scaleY = scaleArray[1].value<double>().value();
    const float scaleZ = scaleArray[2].value<double>().value();
    const glm::vec3 scale = {scaleX, scaleY, scaleZ};

    return {position, rotation, scale};
}
