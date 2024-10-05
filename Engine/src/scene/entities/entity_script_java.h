#pragma once
#include "application.h"
#include "scene/entities/entity_script.h"

class EntityScriptJavaImpl;

class EntityScriptJava : public EntityScript {
    friend EntityScriptJavaImpl;

public:
    static Ref<EntityScriptJava> create(const Application* app, const Ref<Entity>& entity, const std::string& class_name);

protected:
    EntityScriptJava(const Application* app, const Ref<Entity>& entity) : EntityScript(app, entity) {}
};
