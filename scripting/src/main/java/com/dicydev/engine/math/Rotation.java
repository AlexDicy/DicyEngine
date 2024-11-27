package com.dicydev.engine.math;

// Keep in sync with scene/math/Rotation.h

import com.dicydev.engine.components.Transform;

import java.nio.ByteBuffer;

/**
 * Rotation stores pitch, yaw and roll as degrees.
 * Positive values for pitch rotate X axis up, (up and down)
 * Positive values for yaw rotate Y axis right, (turning your head)
 * Positive values for roll rotate Z axis clockwise, (tilting your head)
 * <p>
 * The actual rotation is stored as a quaternion, which is only updated when needed.
 */
public class Rotation {
    private float pitch;
    private float yaw;
    private float roll;
    private final Quaternion quaternion;
    private boolean recalculateQuaternion = true;

    public Rotation() {
        this(0, 0, 0);
    }

    public Rotation(float pitch, float yaw, float roll) {
        this.pitch = pitch;
        this.yaw = yaw;
        this.roll = roll;
        quaternion = new Quaternion();
    }

    public Rotation(Quaternion quaternion) {
        this.quaternion = quaternion;
        recalculateAngles();
    }

    protected Rotation(Quaternion quaternion, boolean recalculateAngles) {
        this.quaternion = quaternion;
        if (recalculateAngles) {
            recalculateAngles();
        }
    }

    public static Rotation fromBuffer(ByteBuffer buffer, int offset) {
        return new BufferRotation(buffer, offset);
    }

    /**
     * When a {@link Rotation} is created from a {@link Transform} buffer, the {@link Rotation} will invalidate
     * the transformation local and global matrices, required for updating child entities.
     */
    public static Rotation fromTransformBuffer(ByteBuffer buffer, int offset, Transform owner) {
        return new BufferTransformRotation(buffer, offset, owner);
    }

    public float getPitch() {
        return pitch;
    }

    public float getYaw() {
        return yaw;
    }

    public float getRoll() {
        return roll;
    }

    public void setPitch(float pitch) {
        this.pitch = pitch;
        setRecalculateQuaternion(true);
    }

    public void setYaw(float yaw) {
        this.yaw = yaw;
        setRecalculateQuaternion(true);
    }

    public void setRoll(float roll) {
        this.roll = roll;
        setRecalculateQuaternion(true);
    }

    public Quaternion getQuaternion() {
        if (shouldRecalculateQuaternion()) {
            setRecalculateQuaternion(false);
            quaternion.setFromAngles((float) Math.toRadians(getPitch()), (float) Math.toRadians(getYaw()), (float) Math.toRadians(getRoll()));
        }
        return quaternion;
    }

    public void addPitch(float pitch) {
        setPitch(getPitch() + pitch);
    }

    public void addYaw(float yaw) {
        setYaw(getYaw() + yaw);
    }

    public void addRoll(float roll) {
        setRoll(getRoll() + roll);
    }

    public void set(Rotation other) {
        setPitch(other.getPitch());
        setYaw(other.getYaw());
        setRoll(other.getRoll());
    }

    public void set(Vector3 angles) {
        setPitch(angles.getX());
        setYaw(angles.getY());
        setRoll(angles.getZ());
    }

    protected boolean shouldRecalculateQuaternion() {
        return recalculateQuaternion;
    }

    protected void setRecalculateQuaternion(boolean recalculate) {
        this.recalculateQuaternion = recalculate;
    }

    protected void recalculateAngles() {
        Vector3 radians = quaternion.toAngles();
        setPitch((float) Math.toDegrees(radians.getX()));
        setYaw((float) Math.toDegrees(radians.getY()));
        setRoll((float) Math.toDegrees(radians.getZ()));
    }


    /**
     * A {@link Rotation} implementation that reads and writes to a {@link ByteBuffer}.
     * Used to access the engine's native memory directly.
     */
    private static class BufferRotation extends Rotation {
        private final ByteBuffer buffer;
        private final int offset;

        private static final int OFFSET_PITCH = 0;
        private static final int OFFSET_YAW = 4;
        private static final int OFFSET_ROLL = 8;
        private static final int OFFSET_QUATERNION = 12;
        private static final int OFFSET_RECALCULATE_QUATERNION = 12 + Quaternion.BYTES;

        /**
         * Constructs a new {@link BufferRotation} with the specified buffer and offset.
         * <p>
         * Use {@link Rotation#fromBuffer(ByteBuffer, int)} to create a new {@link BufferRotation}.
         *
         * @param buffer the buffer
         * @param offset the offset in the buffer
         */
        protected BufferRotation(ByteBuffer buffer, int offset) {
            super(Quaternion.fromBuffer(buffer, offset + OFFSET_QUATERNION), false);
            this.buffer = buffer;
            this.offset = offset;
        }

        @Override
        public float getPitch() {
            return buffer.getFloat(offset + OFFSET_PITCH);
        }

        @Override
        public float getYaw() {
            return buffer.getFloat(offset + OFFSET_YAW);
        }

        @Override
        public float getRoll() {
            return buffer.getFloat(offset + OFFSET_ROLL);
        }

        @Override
        public void setPitch(float pitch) {
            buffer.putFloat(offset + OFFSET_PITCH, pitch);
            setRecalculateQuaternion(true);
        }

        @Override
        public void setYaw(float yaw) {
            buffer.putFloat(offset + OFFSET_YAW, yaw);
            setRecalculateQuaternion(true);
        }

        @Override
        public void setRoll(float roll) {
            buffer.putFloat(offset + OFFSET_ROLL, roll);
            setRecalculateQuaternion(true);
        }

        @Override
        protected boolean shouldRecalculateQuaternion() {
            return buffer.getInt(offset + OFFSET_RECALCULATE_QUATERNION) != 0;
        }

        @Override
        protected void setRecalculateQuaternion(boolean recalculate) {
            buffer.putInt(offset + OFFSET_RECALCULATE_QUATERNION, recalculate ? 1 : 0);
        }
    }

    /**
     * A {@link Rotation} implementation owned by a {@link Transform} that reads and writes to a {@link ByteBuffer}.
     * Used to access the engine's native memory directly and invalidate the transformation matrices.
     */
    private static class BufferTransformRotation extends BufferRotation {
        private final Transform owner;

        /**
         * Constructs a new {@link BufferTransformRotation} with the specified buffer, offset and owning transformation.
         * <p>
         * Use {@link Rotation#fromTransformBuffer(ByteBuffer, int, Transform)} to create a new {@link BufferTransformRotation}.
         *
         * @param buffer the buffer
         * @param offset the offset in the buffer
         * @param owner  the transformation that owns this rotation
         */
        protected BufferTransformRotation(ByteBuffer buffer, int offset, Transform owner) {
            super(buffer, offset);
            this.owner = owner;
        }

        @Override
        protected void setRecalculateQuaternion(boolean recalculate) {
            super.setRecalculateQuaternion(recalculate);
            if (recalculate) {
                owner.invalidate();
            }
        }
    }
}