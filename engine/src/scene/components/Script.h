#pragma once
#include "scene/entities/EntityScript.h"

class Script {
public:
    Script(const Ref<EntityScript>& script) : entityScript(script) {}
    Script(const std::string& name, const Ref<Application>& app, const Ref<Entity>& entity);

    const std::string& getName() const {
        return this->name;
    }

    const Ref<EntityScript>& getEntityScript() const {
        return this->entityScript;
    }

private:
    std::string name;
    Ref<EntityScript> entityScript;
};
