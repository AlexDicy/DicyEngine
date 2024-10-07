package com.dicydev.engine.scene.scripts;

import com.dicydev.engine.components.Transform;
import com.dicydev.engine.input.Input;
import com.dicydev.engine.input.InputCode;


public class CameraScript extends EntityScript {

    public CameraScript() {
        System.out.println("CameraScript constructor");
        Input.setAction("change_camera", InputCode.KEY_O);
        Input.setAction("move_camera_up", InputCode.KEY_E);
        Input.setAction("move_camera_down", InputCode.KEY_Q);
        Input.setAction("move_camera_faster", InputCode.KEY_LEFT_SHIFT);
    }

    public void onUpdate(float deltaTime) {
        var transform = getComponent(Transform.class);
        System.out.println("CameraScript onUpdate: " + transform.getPosition().getY());
    }
}
