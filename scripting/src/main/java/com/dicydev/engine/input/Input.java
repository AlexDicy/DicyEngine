package com.dicydev.engine.input;

public class Input {
    public static void setAction(String action, InputCode inputCode) {
        setAction(action, inputCode.getValue());
    }

    private static native void setAction(String action, int inputCode);

    public static native boolean isActionPressed(String action);
}
