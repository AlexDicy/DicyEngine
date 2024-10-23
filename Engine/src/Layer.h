#pragma once
#include "Context.h"

class Layer {
public:
    virtual ~Layer() = default;

    virtual void update(const std::unique_ptr<Context> &ctx) {
    }
};
