#pragma once

class Physics {
public:
    virtual ~Physics() = default;
    virtual void init() = 0;
    virtual void update(float deltaTime, int steps) = 0;
};
