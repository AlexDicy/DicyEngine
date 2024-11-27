#pragma once
#include "scene/entities/EntityScript.h"

class EntityScriptRegistry {
public:
    EntityScriptRegistry() = default;

    template <typename T>
    void registerScript(const std::string& name) {
        this->scripts[name] = [](const Ref<Application>& app, const Ref<Entity>& entity) {
            return std::make_shared<T>(app, entity);
        };
    }

    Ref<EntityScript> createInstance(const std::string& name, const Ref<Application>& app, const Ref<Entity>& entity) const {
        try {
            return this->scripts.at(name)(app, entity);
        } catch (const std::out_of_range&) {
            DE_WARN("Script not found: {0}", name);
            throw;
        }
    }

private:
    std::unordered_map<std::string, std::function<Ref<EntityScript>(const Ref<Application>&, const Ref<Entity>&)>> scripts;
};
