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
        return Rotation.fromTransformBuffer(buffer, 12, this);
    }

    public void setRotation(Rotation rotation) {
        Rotation bufferRotation = getRotation();
        bufferRotation.setPitch(rotation.getPitch());
        bufferRotation.setYaw(rotation.getYaw());
        bufferRotation.setRoll(rotation.getRoll());
    }

    public Vector3 getScale() {
        return Vector3.fromBuffer(buffer, 44);
    }

    public void setScale(Vector3 scale) {
        buffer.putFloat(44, scale.getX());
        buffer.putFloat(48, scale.getY());
        buffer.putFloat(52, scale.getZ());
    }

    public void invalidate() {
        invalidateLocal();
        invalidateGlobal(buffer);
    }

    public void invalidateLocal() {
        buffer.put(184, (byte) 1);
    }

    public static native void invalidateGlobal(ByteBuffer buffer);
}
