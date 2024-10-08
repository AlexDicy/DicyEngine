package com.dicydev.engine.scene.scripts;

import com.dicydev.engine.annotations.CalledByNative;
import com.dicydev.engine.components.Component;
import com.dicydev.engine.components.NativeRegistry;
import com.dicydev.engine.scene.Scene;

public abstract class EntityScript {
    private Scene scene;
    private long registryPointer;
    private int entityId;

    @CalledByNative
    public void setEntityInfo(Scene scene, long registryPointer, int entityId) {
        this.scene = scene;
        this.registryPointer = registryPointer;
        this.entityId = entityId;
        this.init();
    }

    public void init() {}

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

    public Scene getScene() {
        return scene;
    }

    public <C extends Component> C getComponent(Class<C> componentType) {
        return NativeRegistry.getComponent(registryPointer, entityId, componentType);
    }
}
