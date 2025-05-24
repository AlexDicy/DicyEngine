#pragma once

class Physics {
public:
    virtual ~Physics() = default;
    virtual void init() = 0;
};
