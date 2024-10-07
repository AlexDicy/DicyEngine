package com.dicydev.engine.scene.scripts;

import com.dicydev.engine.annotations.CalledByNative;
import com.dicydev.engine.components.Component;
import com.dicydev.engine.components.NativeRegistry;

public abstract class EntityScript {
    private long registryPointer;
    private int entityId;

    @CalledByNative
    public void setEntityInfo(long registryPointer, int entityId) {
        this.registryPointer = registryPointer;
        this.entityId = entityId;
        //System.out.println("EntityScript constructor: " + registryPointer + ", " + entityId);
    }

    @CalledByNative
    public void onUpdate(float deltaTime) {}

    @CalledByNative
    public void onSpawn() {}

    @CalledByNative
    public void onDestroy() {}

    @CalledByNative
    public void onAwake() {}

    @CalledByNative
    public void onSleep() {}

    public <C extends Component> C getComponent(Class<C> componentType) {
        return NativeRegistry.getComponent(registryPointer, entityId, componentType);
    }
}
