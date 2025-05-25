#pragma once

enum class ShapeType { SPHERE, BOX };

class PhysicsShape {
public:
    virtual ~PhysicsShape() = default;

    virtual ShapeType getType() const = 0;
};
