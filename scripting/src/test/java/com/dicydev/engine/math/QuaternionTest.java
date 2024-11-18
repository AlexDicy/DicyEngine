package com.dicydev.engine.math;

import org.junit.jupiter.api.Test;

import static com.dicydev.engine.math.Quaternion.EPSILON;
import static org.junit.jupiter.api.Assertions.*;

class QuaternionTest {

    @Test
    void testSetFromAngles() {
        Quaternion q = new Quaternion();
        q.setFromAngles(0.0f, 0.0f, 0.0f);
        assertEquals(0.0f, q.getX(), EPSILON);
        assertEquals(0.0f, q.getY(), EPSILON);
        assertEquals(0.0f, q.getZ(), EPSILON);
        assertEquals(1.0f, q.getW(), EPSILON);

        q.setFromAngles(-1.9415927f, -0.25840744f, 2.4584072f);
        assertEquals(-0.20562552f, q.getX(), EPSILON);
        assertEquals(-0.79553854f, q.getY(), EPSILON);
        assertEquals(0.49196157f, q.getZ(), EPSILON);
        assertEquals(0.2877679f, q.getW(), EPSILON);
    }

    @Test
    void testToAngles() {
        Quaternion q = new Quaternion(0.0f, 0.0f, 0.0f);
        Vector3 angles = q.toAngles();
        assertEquals(0.0f, angles.getX(), EPSILON);
        assertEquals(0.0f, angles.getY(), EPSILON);
        assertEquals(0.0f, angles.getZ(), EPSILON);

        q = new Quaternion(0.0f, (float) (Math.PI / 2.0f), 0.0f);
        angles = q.toAngles();
        assertEquals(0.0f, angles.getX(), EPSILON);
        assertEquals((float) (Math.PI / 2.0f), angles.getY(), EPSILON);
        assertEquals(0.0f, angles.getZ(), EPSILON);

        q = new Quaternion(-0.20562552f, -0.79553854f, 0.49196157f, 0.2877679f);
        angles = q.toAngles();
        assertEquals(-1.9415927f, angles.getX(), EPSILON);
        assertEquals(-0.2584074f, angles.getY(), EPSILON);
        assertEquals(2.4584072f, angles.getZ(), EPSILON);
    }
}