#pragma once
#include "Serializer.h"

class TransformSerializer : public Serializer<Transform> {
public:
    void serialize(Transform& transform, toml::table& out) override;
};
