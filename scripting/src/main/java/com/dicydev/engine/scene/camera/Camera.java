package com.dicydev.engine.scene.camera;

import com.dicydev.engine.annotations.CalledByNative;
import com.dicydev.engine.math.Rotation;
import com.dicydev.engine.math.Vector3;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;

public class Camera {
    private final long pointer;

    @CalledByNative
    private Camera(long pointer) {
        this.pointer = pointer;
    }

    private static native Camera getMainCamera();

    public Vector3 getPosition() {
        return Vector3.fromBuffer(getPosition(pointer).order(ByteOrder.nativeOrder()), 0);
    }

    private static native ByteBuffer getPosition(long pointer);

    public Rotation getRotation() {
        return Rotation.fromBuffer(getRotation(pointer).order(ByteOrder.nativeOrder()), 0);
    }

    private static native ByteBuffer getRotation(long pointer);
}
