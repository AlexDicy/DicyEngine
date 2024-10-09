package com.dicydev.engine.input;

import com.dicydev.engine.annotations.CalledByNative;

@FunctionalInterface
public interface InputAxisCallback {

    /**
     * Called when the axis is updated.
     *
     * @param delta The change in the axis value.
     */
    @CalledByNative
    void callback(float delta);
}
