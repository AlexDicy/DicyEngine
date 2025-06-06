#pragma once
#include "Layer.h"

class PhysicsLayer : public Layer {
public:
    explicit PhysicsLayer(const std::unique_ptr<Context>& ctx) : Layer(ctx) {}

    void play(const std::unique_ptr<Context>& ctx) override {}
    void update(const std::unique_ptr<Context>& ctx) override;
    void stop(const std::unique_ptr<Context>& ctx) override {}
};
