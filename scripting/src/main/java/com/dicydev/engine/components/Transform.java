package com.dicydev.engine.components;

import com.dicydev.engine.math.Quaternion;
import com.dicydev.engine.math.Rotation;
import com.dicydev.engine.math.Vector3;

import java.nio.ByteBuffer;

// Keep in sync with scene/components/Transform.h
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

    public Rotation getRotation() {
        return Rotation.fromBuffer(buffer, 12);
    }

    public void setRotation(Rotation rotation) {
        Rotation bufferRotation = getRotation();
        bufferRotation.setPitch(rotation.getPitch());
        bufferRotation.setYaw(rotation.getYaw());
        bufferRotation.setRoll(rotation.getRoll());
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
