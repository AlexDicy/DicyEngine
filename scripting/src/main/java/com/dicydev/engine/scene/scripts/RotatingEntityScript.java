package com.dicydev.engine.scene.scripts;

import com.dicydev.engine.components.Transform;
import com.dicydev.engine.math.Rotation;

public class RotatingEntityScript extends EntityScript {
    private Transform transform;

    @Override
    public void init() {
        transform = getComponent(Transform.class);
    }

    @Override
    public void onUpdate(float deltaTime) {
        Rotation rotation = transform.getRotation();
        rotation.setYaw(rotation.getYaw() + 5.0f * deltaTime);
//        this->transform->setRotation(rotation);
    }
}
