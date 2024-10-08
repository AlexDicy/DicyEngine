package com.dicydev.engine.components;

import com.dicydev.engine.math.Vector3;

import java.nio.ByteBuffer;

// Keep in sync with scene/components/transform.h
public class Transform extends Component {
    public Transform(ByteBuffer buffer) {
        super(buffer);
    }

    public Vector3 getPosition() {
        return Vector3.fromBuffer(buffer, 0);
    }

    public void setPosition(Vector3 position) {
        buffer.putFloat(0, position.getX());
        buffer.putFloat(4, position.getY());
        buffer.putFloat(8, position.getZ());
    }

    // TODO: replace Vector3 with a Rotation class
    public Vector3 getRotation() {
        return Vector3.fromBuffer(buffer, 12);
    }

    public void setRotation(Vector3 rotation) {
        buffer.putFloat(12, rotation.getX());
        buffer.putFloat(16, rotation.getY());
        buffer.putFloat(20, rotation.getZ());
    }

    public Vector3 getScale() {
        return Vector3.fromBuffer(buffer, 24);
    }

    public void setScale(Vector3 scale) {
        buffer.putFloat(24, scale.getX());
        buffer.putFloat(28, scale.getY());
        buffer.putFloat(32, scale.getZ());
    }
}
