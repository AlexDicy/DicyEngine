#pragma once
#include "Serializer.h"

class RotationSerializer : public Serializer<Rotation> {
public:
    void serialize(Rotation& rotation, toml::table& out) override;
};
