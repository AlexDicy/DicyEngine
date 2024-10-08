package com.dicydev.engine.scene.camera;

import com.dicydev.engine.annotations.CalledByNative;
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
        return Vector3.fromBuffer(getPosition(pointer).order(ByteOrder.nativeOrder()), 0); // TODO: check if needed
    }

    private static native ByteBuffer getPosition(long pointer);

    public Vector3 getRotation() {
        return Vector3.fromBuffer(getRotation(pointer).order(ByteOrder.nativeOrder()), 0); // TODO: check if needed
    }

    private static native ByteBuffer getRotation(long pointer);
}
