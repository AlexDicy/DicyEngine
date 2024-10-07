package com.dicydev.engine.math;

import java.nio.ByteBuffer;

public class Vector3 {
    private static final Vector3 ZERO = new Vector3(0, 0, 0);
    private static final Vector3 ONE = new Vector3(1, 1, 1);
    private static final Vector3 UP = new Vector3(0, 1, 0);
    private static final Vector3 DOWN = new Vector3(0, -1, 0);
    private static final Vector3 LEFT = new Vector3(-1, 0, 0);
    private static final Vector3 RIGHT = new Vector3(1, 0, 0);
    private static final Vector3 FORWARD = new Vector3(0, 0, 1);
    private static final Vector3 BACKWARD = new Vector3(0, 0, -1);

    /**
     * The x, y, and z components of this vector.
     * Not used in {@link BufferVector3}.
     */
    private float x;
    private float y;
    private float z;

    public Vector3() {
        this(0, 0, 0);
    }

    public Vector3(float scalar) {
        this(scalar, scalar, scalar);
    }

    public Vector3(float x, float y, float z) {
        this.x = x;
        this.y = y;
        this.z = z;
    }

    public Vector3(Vector3 other) {
        this(other.getX(), other.getY(), other.getZ());
    }

    public static Vector3 fromBuffer(ByteBuffer buffer, int offset) {
        return new BufferVector3(buffer, offset);
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

    public void setX(float x) {
        this.x = x;
    }

    public void setY(float y) {
        this.y = y;
    }

    public void setZ(float z) {
        this.z = z;
    }

    public Vector3 add(Vector3 other) {
        setX(getX() + other.getX());
        setY(getY() + other.getY());
        setZ(getZ() + other.getZ());
        return this;
    }

    public Vector3 subtract(Vector3 other) {
        setX(getX() - other.getX());
        setY(getY() - other.getY());
        setZ(getZ() - other.getZ());
        return this;
    }

    public Vector3 multiply(Vector3 other) {
        setX(getX() * other.getX());
        setY(getY() * other.getY());
        setZ(getZ() * other.getZ());
        return this;
    }

    public Vector3 divide(Vector3 other) {
        setX(getX() / other.getX());
        setY(getY() / other.getY());
        setZ(getZ() / other.getZ());
        return this;
    }

    public Vector3 add(float scalar) {
        setX(getX() + scalar);
        setY(getY() + scalar);
        setZ(getZ() + scalar);
        return this;
    }

    public Vector3 subtract(float scalar) {
        setX(getX() - scalar);
        setY(getY() - scalar);
        setZ(getZ() - scalar);
        return this;
    }

    public Vector3 multiply(float scalar) {
        setX(getX() * scalar);
        setY(getY() * scalar);
        setZ(getZ() * scalar);
        return this;
    }

    public Vector3 divide(float scalar) {
        setX(getX() / scalar);
        setY(getY() / scalar);
        setZ(getZ() / scalar);
        return this;
    }

    public Vector3 normalize() {
        return divide(length());
    }

    public float length() {
        return (float) Math.sqrt(getX() * getX() + getY() * getY() + getZ() * getZ());
    }

    public float dot(Vector3 other) {
        return getX() * other.getX() + getY() * other.getY() + getZ() * other.getZ();
    }
}


class BufferVector3 extends Vector3 {
    private final ByteBuffer buffer;
    private final int offset;

    /**
     * Constructs a new {@link BufferVector3} with the specified buffer and offset.
     * <p>
     * Use {@link Vector3#fromBuffer(ByteBuffer, int)} to create a new {@link BufferVector3}.
     *
     * @param buffer the buffer
     * @param offset the offset in the buffer
     */
    protected BufferVector3(ByteBuffer buffer, int offset) {
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
}
