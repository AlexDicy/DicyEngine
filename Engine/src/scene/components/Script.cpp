#include "pch/enginepch.h"
#include "Script.h"

#include "Application.h"


Script::Script(const std::string& name, const Ref<Application>& app, const Ref<Entity>& entity) {
    this->name = name;
    this->entityScript = app->getEntityScriptRegistry()->createInstance(name, app, entity);
}
