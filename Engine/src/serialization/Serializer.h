#pragma once
#include <toml++/toml.hpp>

template <typename T>
class Serializer {
public:
    virtual ~Serializer() = default;

    virtual void serialize(T& data, toml::table& out) = 0;
};
