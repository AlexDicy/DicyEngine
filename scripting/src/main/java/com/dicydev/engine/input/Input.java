package com.dicydev.engine.input;

public class Input {
    public static void setAction(String action, InputCode inputCode) {
        setAction(action, inputCode.getValue());
    }

    private static native void setAction(String action, int inputCode);

    public static native void bindAxis(String axis, InputAxisCallback callback);

    public static native void bindActionPressed(String action, InputActionCallback callback);

    public static native void bindActionReleased(String action, InputActionCallback callback);

    public static native boolean isActionPressed(String action);
}
