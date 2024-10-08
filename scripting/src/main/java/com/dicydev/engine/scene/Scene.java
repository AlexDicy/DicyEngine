package com.dicydev.engine.scene;

import com.dicydev.engine.annotations.CalledByNative;
import com.dicydev.engine.scene.camera.Camera;

public class Scene {
    @CalledByNative
    private final long pointer;

    @CalledByNative
    private Scene(long pointer) {
        this.pointer = pointer;
    }

    public native Camera getCamera();
}
