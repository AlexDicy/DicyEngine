#pragma once

class Buffer {
public:
    virtual ~Buffer() = default;

    virtual void bind() const = 0;
};
