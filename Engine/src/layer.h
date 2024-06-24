#pragma once
#include "rendering/renderer.h"

class Layer {
public:
    virtual ~Layer() = default;

    virtual void update(const std::unique_ptr<Renderer>& renderer) {
    }
};
