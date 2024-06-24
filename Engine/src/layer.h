#pragma once
#include "context.h"

class Layer {
public:
    virtual ~Layer() = default;

    virtual void update(const std::unique_ptr<Context> &ctx) {
    }
};
