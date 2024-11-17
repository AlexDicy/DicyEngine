package com.dicydev.engine.components;

import java.lang.reflect.InvocationTargetException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.HashMap;
import java.util.Map;

public class NativeRegistry {
    private static final Map<Class<? extends Component>, Integer> componentTypeMap = new HashMap<>();

    public static <C extends Component> C getComponent(long registryPointer, int entityId, Class<C> componentType) {
        try {
            int componentTypeId = componentTypeMap.get(componentType);
            ByteBuffer buffer = getComponentBuffer(registryPointer, entityId, componentTypeId);
            buffer.order(ByteOrder.nativeOrder());
            return componentType.getConstructor(ByteBuffer.class).newInstance(buffer);
        } catch (InstantiationException | IllegalAccessException | InvocationTargetException | NoSuchMethodException e) {
            throw new RuntimeException(e);
        }
    }

    private static native ByteBuffer getComponentBuffer(long registryPointer, int entityId, int componentTypeId);

    // Keep in sync with scene/components/ComponentTypes.h
    static {
        componentTypeMap.put(Transform.class, 0);
    }
}
