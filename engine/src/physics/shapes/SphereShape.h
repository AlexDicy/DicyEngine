#pragma once

class SphereShape : public PhysicsShape {
public:
    explicit SphereShape(const float radius) : PhysicsShape(), radius(radius) {}

    ShapeType getType() const override {
        return ShapeType::SPHERE;
    }

    float getRadius() const {
        return radius;
    }

private:
    float radius;
};
