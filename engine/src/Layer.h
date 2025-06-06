#pragma once
#include "Context.h"

class Layer {
public:
    explicit Layer(const std::unique_ptr<Context>& ctx) : scene(ctx->scene) {}
    virtual ~Layer() = default;

    virtual void play(const std::unique_ptr<Context>& ctx) = 0;
    virtual void update(const std::unique_ptr<Context>& ctx) = 0;
    virtual void stop(const std::unique_ptr<Context>& ctx) = 0;

protected:
    Ref<Scene> scene;
};
