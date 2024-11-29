#pragma once
#include "Deserializer.h"

#include <toml++/toml.hpp>

class TransformDeserializer : public Deserializer<Transform> {
public:
    static Transform deserialize(const toml::table& in);
};
