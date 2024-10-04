package com.dicydev.engine;

public class EngineBridge {
    private static EngineBridge instance;

    private int counter = 0;

    public static EngineBridge getInstance() {
        if (instance == null) {
            instance = new EngineBridge();
        }
        return instance;
    }

    public void incrementCounter() {
        counter++;
    }

    public int getCounter() {
        return counter;
    }
}
