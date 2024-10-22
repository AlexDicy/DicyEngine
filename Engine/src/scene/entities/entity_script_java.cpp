#include "pch/enginepch.h"
#include "entity_script_java.h"
#include "scripting/java_bridge.h"

class EntityScriptJavaImpl final : public EntityScriptJava {
public:
    EntityScriptJavaImpl(const Ref<Application>& app, const Ref<Entity>& entity, const std::string& class_name) : EntityScriptJava(app, entity) {
        this->java_class = new JavaClass(class_name);

        this->on_update_id = this->java_class->getMethod("onUpdate", "(F)V");
        this->on_spawn_id = this->java_class->getMethod("onSpawn", "()V");
        this->on_destroy_id = this->java_class->getMethod("onDestroy", "()V");
        this->on_awake_id = this->java_class->getMethod("onAwake", "()V");
        this->on_sleep_id = this->java_class->getMethod("onSleep", "()V");

        const jobject scene_object = JavaClass("com/dicydev/engine/scene/Scene").newInstance("(J)V", entity->get_scene().get());
        entt::registry* registry_pointer = entity->get_registry().get();
        this->java_object = this->java_class->newInstance();
        const jmethodID set_entity_info_id = this->java_class->getMethod("setEntityInfo", "(Lcom/dicydev/engine/scene/Scene;JI)V");
        this->java_class->callVoid(this->java_object, set_entity_info_id, scene_object, registry_pointer, entity->get_entity_id());
    }

    ~EntityScriptJavaImpl() override {
        delete this->java_class;
    }

    void on_update(float delta_time) override {
        this->java_class->callVoid(this->java_object, this->on_update_id, delta_time);
    }

    void on_spawn() override {
        this->java_class->callVoid(this->java_object, this->on_spawn_id);
    }

    void on_destroy() override {
        this->java_class->callVoid(this->java_object, this->on_destroy_id);
    }

    void on_awake() override {
        this->java_class->callVoid(this->java_object, this->on_awake_id);
    }

    void on_sleep() override {
        this->java_class->callVoid(this->java_object, this->on_sleep_id);
    }

private:
    JavaClass* java_class;
    jobject java_object;

    jmethodID on_update_id;
    jmethodID on_spawn_id;
    jmethodID on_destroy_id;
    jmethodID on_awake_id;
    jmethodID on_sleep_id;
};

Ref<EntityScriptJava> EntityScriptJava::create(const Ref<Application>& app, const Ref<Entity>& entity, const std::string& class_name) {
    return std::make_shared<EntityScriptJavaImpl>(app, entity, class_name);
}
