package com.dicydev.engine.input;

@FunctionalInterface
public interface InputAxisCallback {

    /**
     * Called when the axis is updated.
     *
     * @param delta The change in the axis value.
     */
    void callback(float delta);
}
