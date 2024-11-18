package com.dicydev.engine.math;

import java.nio.ByteBuffer;

public class Quaternion {
    private float x;
    private float y;
    private float z;
    private float w;

    protected static final float EPSILON = 0.000001f;
    protected static final int BYTES = 4 * Float.BYTES;

    public Quaternion() {
        this(0, 0, 0, 1);
    }

    public Quaternion(float x, float y, float z, float w) {
        this.x = x;
        this.y = y;
        this.z = z;
        this.w = w;
    }

    /**
     * @param pitch The pitch in radians
     * @param yaw   The yaw in radians
     * @param roll  The roll in radians
     */
    public Quaternion(float pitch, float yaw, float roll) {
        setFromAngles(pitch, yaw, roll);
        normalize();
    }

    public static Quaternion fromBuffer(ByteBuffer buffer, int offset) {
        return new BufferQuaternion(buffer, offset);
    }

    public float getX() {
        return x;
    }

    public float getY() {
        return y;
    }

    public float getZ() {
        return z;
    }

    public float getW() {
        return w;
    }

    public void setX(float x) {
        this.x = x;
    }

    public void setY(float y) {
        this.y = y;
    }

    public void setZ(float z) {
        this.z = z;
    }

    public void setW(float w) {
        this.w = w;
    }

    public void setFromAngles(float pitch, float yaw, float roll) {
        double cx = Math.cos(pitch * 0.5f);
        double cy = Math.cos(yaw * 0.5f);
        double cz = Math.cos(roll * 0.5f);
        double sx = Math.sin(pitch * 0.5f);
        double sy = Math.sin(yaw * 0.5f);
        double sz = Math.sin(roll * 0.5f);

        setW((float) (cx * cy * cz + sx * sy * sz));
        setX((float) (sx * cy * cz - cx * sy * sz));
        setY((float) (cx * sy * cz + sx * cy * sz));
        setZ((float) (cx * cy * sz - sx * sy * cz));
    }

    public Vector3 toAngles() {
        return new Vector3(calculatePitch(), calculateYaw(), calculateRoll());
    }

    public void normalize() {
        float x = getX();
        float y = getY();
        float z = getZ();
        float w = getW();
        float length = (float) Math.sqrt(x * x + y * y + z * z + w * w);
        setX(x / length);
        setY(y / length);
        setZ(z / length);
        setW(w / length);
    }

    private float calculatePitch() {
        float x = getX();
        float y = getY();
        float z = getZ();
        float w = getW();
        float floatY = 2 * (y * z + w * x);
        float floatX = w * w - x * x - y * y + z * z;
        if (Math.abs(floatX) < EPSILON && Math.abs(floatY) < EPSILON) {
            return 2 * (float) Math.atan2(x, w);
        }
        return (float) Math.atan2(floatY, floatX);
    }

    private float calculateYaw() {
        float x = getX();
        float y = getY();
        float z = getZ();
        float w = getW();
        return (float) Math.asin(Math.max(-1, Math.min(1, -2 * (x * z - w * y))));
    }

    private float calculateRoll() {
        float x = getX();
        float y = getY();
        float z = getZ();
        float w = getW();
        float floatY = 2 * (x * y + w * z);
        float floatX = w * w + x * x - y * y - z * z;
        if (Math.abs(floatX) < EPSILON && Math.abs(floatY) < EPSILON) {
            return 0;
        }
        return (float) Math.atan2(floatY, floatX);
    }


    /**
     * A {@link Quaternion} implementation that reads and writes to a {@link ByteBuffer}.
     * Used to access the engine's native memory directly.
     */
    private static class BufferQuaternion extends Quaternion {
        private final ByteBuffer buffer;
        private final int offset;

        /**
         * Constructs a new {@link Quaternion.BufferQuaternion} with the specified buffer and offset.
         * <p>
         * Use {@link Quaternion#fromBuffer(ByteBuffer, int)} to create a new {@link Quaternion.BufferQuaternion}.
         *
         * @param buffer the buffer
         * @param offset the offset in the buffer
         */
        protected BufferQuaternion(ByteBuffer buffer, int offset) {
            this.buffer = buffer;
            this.offset = offset;
        }

        @Override
        public float getX() {
            return buffer.getFloat(offset);
        }

        @Override
        public float getY() {
            return buffer.getFloat(offset + 4);
        }

        @Override
        public float getZ() {
            return buffer.getFloat(offset + 8);
        }

        @Override
        public float getW() {
            return buffer.getFloat(offset + 12);
        }

        @Override
        public void setX(float x) {
            buffer.putFloat(offset, x);
        }

        @Override
        public void setY(float y) {
            buffer.putFloat(offset + 4, y);
        }

        @Override
        public void setZ(float z) {
            buffer.putFloat(offset + 8, z);
        }

        @Override
        public void setW(float w) {
            buffer.putFloat(offset + 12, w);
        }
    }
}
