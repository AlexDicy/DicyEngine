package com.dicydev.engine.input;

import com.dicydev.engine.annotations.CalledByNative;

@FunctionalInterface
public interface InputActionCallback {

    /**
     * Called when an action is pressed or released.
     */
    @CalledByNative
    void callback();
}
