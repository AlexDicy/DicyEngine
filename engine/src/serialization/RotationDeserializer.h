#pragma once
#include "Deserializer.h"

#include <toml++/toml.hpp>

class RotationDeserializer : public Deserializer<Rotation> {
public:
    static Rotation deserialize(const toml::table& in);
};
