#pragma once
#include "scene/entities/EntityScript.h"

class Application;
class EntityScriptJavaImpl;

class EntityScriptJava : public EntityScript {
    friend EntityScriptJavaImpl;

public:
    static Ref<EntityScriptJava> create(const Ref<Application>& app, const Ref<Entity>& entity, const std::string& className);

    ScriptType getType() const override {
        return ScriptType::JVM;
    }

protected:
    EntityScriptJava(const Ref<Application>& app, const Ref<Entity>& entity) : EntityScript(app, entity) {}
};
