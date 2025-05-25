#pragma once

class BoxShape : public PhysicsShape {
public:
    explicit BoxShape(const glm::vec3 dimensions) : PhysicsShape(), dimensions(dimensions) {}

    ShapeType getType() const override {
        return ShapeType::BOX;
    }

    const glm::vec3& getDimensions() const {
        return dimensions;
    }

private:
    glm::vec3 dimensions;
};
