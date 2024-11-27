#include "pch/enginepch.h"
#include "EntityScriptJava.h"
#include "scripting/JavaBridge.h"

class EntityScriptJavaImpl final : public EntityScriptJava {
public:
    EntityScriptJavaImpl(const Ref<Application>& app, const Ref<Entity>& entity, const std::string& className) : EntityScriptJava(app, entity) {
        std::string javaClassName = className;
        std::ranges::replace(javaClassName, '.', '/');
        this->javaClass = new JavaClass(javaClassName);

        this->onUpdateId = this->javaClass->getMethod("onUpdate", "(F)V");
        this->onSpawnId = this->javaClass->getMethod("onSpawn", "()V");
        this->onDestroyId = this->javaClass->getMethod("onDestroy", "()V");
        this->onAwakeId = this->javaClass->getMethod("onAwake", "()V");
        this->onSleepId = this->javaClass->getMethod("onSleep", "()V");

        const jobject sceneObject = JavaClass("com/dicydev/engine/scene/Scene").newInstance("(J)V", entity->getScene().get());
        entt::registry* registryPointer = entity->getRegistry().get();
        this->javaObject = this->javaClass->newInstance();
        const jmethodID setEntityInfoId = this->javaClass->getMethod("setEntityInfo", "(Lcom/dicydev/engine/scene/Scene;JI)V");
        this->javaClass->callVoid(this->javaObject, setEntityInfoId, sceneObject, registryPointer, entity->getEntityId());
    }

    ~EntityScriptJavaImpl() override {
        delete this->javaClass;
    }

    void onUpdate(const float deltaTime) override {
        this->javaClass->callVoid(this->javaObject, this->onUpdateId, deltaTime);
    }

    void onSpawn() override {
        this->javaClass->callVoid(this->javaObject, this->onSpawnId);
    }

    void onDestroy() override {
        this->javaClass->callVoid(this->javaObject, this->onDestroyId);
    }

    void onAwake() override {
        this->javaClass->callVoid(this->javaObject, this->onAwakeId);
    }

    void onSleep() override {
        this->javaClass->callVoid(this->javaObject, this->onSleepId);
    }

private:
    JavaClass* javaClass;
    jobject javaObject;

    jmethodID onUpdateId;
    jmethodID onSpawnId;
    jmethodID onDestroyId;
    jmethodID onAwakeId;
    jmethodID onSleepId;
};

Ref<EntityScriptJava> EntityScriptJava::create(const Ref<Application>& app, const Ref<Entity>& entity, const std::string& className) {
    return std::make_shared<EntityScriptJavaImpl>(app, entity, className);
}
