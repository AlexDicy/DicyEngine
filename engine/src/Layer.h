#pragma once
#include "Context.h"

class Layer {
public:
    virtual ~Layer() = default;

    virtual void play(const std::unique_ptr<Context> &ctx) = 0;
    virtual void update(const std::unique_ptr<Context> &ctx) = 0;
    virtual void stop(const std::unique_ptr<Context> &ctx) = 0;
};
