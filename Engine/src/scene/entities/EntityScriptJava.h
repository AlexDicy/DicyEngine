#pragma once
#include "Application.h"
#include "scene/entities/EntityScript.h"

class EntityScriptJavaImpl;

class EntityScriptJava : public EntityScript {
    friend EntityScriptJavaImpl;

public:
    static Ref<EntityScriptJava> create(const Ref<Application>& app, const Ref<Entity>& entity, const std::string& className);

protected:
    EntityScriptJava(const Ref<Application>& app, const Ref<Entity>& entity) : EntityScript(app, entity) {}
};
