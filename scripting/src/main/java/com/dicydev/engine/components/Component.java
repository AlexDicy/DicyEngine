package com.dicydev.engine.components;

import java.nio.ByteBuffer;

public abstract class Component {
    protected final ByteBuffer buffer;

    public Component(ByteBuffer buffer) {
        this.buffer = buffer;
    }
}
