package com.dicydev.engine.scene.scripts;

import com.dicydev.engine.components.Transform;
import com.dicydev.engine.input.Input;
import com.dicydev.engine.input.InputCode;
import com.dicydev.engine.math.Vector3;
import com.dicydev.engine.scene.camera.Camera;


public class CameraScript extends EntityScript {
    private Transform transform;
    private Vector3 position;
    private Vector3 velocity = new Vector3(0);
    private boolean moveFaster = false;
    private final float sensitivity = 0.16f;
    private final float dampingFactor = 10.0f;
    private final float baseAcceleration = 36.0f;

    public CameraScript() {
        Input.setAction("move_camera_up", InputCode.KEY_E);
        Input.setAction("move_camera_down", InputCode.KEY_Q);
        Input.setAction("move_camera_faster", InputCode.KEY_LEFT_SHIFT);

        Input.bindAxis("look_x", (float deltaX) -> {
            if (Input.isActionPressed("right_click")) {
                transform.getRotation().addYaw(deltaX * sensitivity);
            }
        });

        Input.bindAxis("look_y", (float deltaY) -> {
            if (Input.isActionPressed("right_click")) {
                float pitch = Math.clamp(transform.getRotation().getPitch() + deltaY * sensitivity, -90.0f, 90.0f);
                transform.getRotation().setPitch(pitch);
            }
        });

        Input.bindActionPressed("move_camera_faster", () -> this.moveFaster = true);

        Input.bindActionReleased("move_camera_faster", () -> this.moveFaster = false);
    }

    @Override
    public void init() {
        transform = getComponent(Transform.class);
        position = transform.getPosition();
    }

    @Override
    public void onUpdate(float deltaTime) {
        // slow down
        velocity = velocity.multiply(Math.max(0.0f, 1.0f - dampingFactor * deltaTime));
        // direction
        double yaw = Math.toRadians(transform.getRotation().getYaw());
        var forward = new Vector3((float) Math.sin(yaw), 0.0f, (float) Math.cos(yaw));
        var right = new Vector3(forward.getZ(), 0.0f, -forward.getX());

        var movementDirection = new Vector3();
        if (Input.isActionPressed("move_left")) {
            movementDirection = movementDirection.subtract(right);
        }
        if (Input.isActionPressed("move_right")) {
            movementDirection = movementDirection.add(right);
        }
        if (Input.isActionPressed("move_forward")) {
            movementDirection = movementDirection.add(forward);
        }
        if (Input.isActionPressed("move_backward")) {
            movementDirection = movementDirection.subtract(forward);
        }
        if (Input.isActionPressed("move_camera_up")) {
            movementDirection.addY(1.0f);
        }
        if (Input.isActionPressed("move_camera_down")) {
            movementDirection.subtractY(1.0f);
        }

        if (movementDirection.length() > 0.0f) {
            float acceleration = moveFaster ? baseAcceleration * 2.0f : baseAcceleration;
            velocity = velocity.add(movementDirection.normalize().multiply(acceleration * deltaTime));
        }

        position.set(position.add(velocity.multiply(deltaTime)));

        Camera camera = getScene().getCamera();
        camera.getPosition().set(position);
        Vector3 angles = transform.getRotation().getQuaternion().toAngles().toDegrees();
        camera.getRotation().set(angles);
    }
}
