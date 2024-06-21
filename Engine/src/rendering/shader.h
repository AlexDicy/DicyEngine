#pragma once

class Shader {
public:
    virtual ~Shader() = default;

    virtual void bind() const = 0;
};
