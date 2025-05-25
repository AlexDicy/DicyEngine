#pragma once
#include "scene/entities/EntityScript.h"
#include "scene/entities/EntityScriptJava.h"

class EntityScriptRegistry {
public:
    EntityScriptRegistry() = default;

    template <typename T>
    void registerScriptNative(const std::string& name) {
        this->scripts[name] = [](const Ref<Application>& app, const Ref<Entity>& entity) {
            return std::make_shared<T>(app, entity);
        };
    }

    void registerScriptJava(const std::string& name) {
        this->scripts[name] = [name](const Ref<Application>& app, const Ref<Entity>& entity) {
            return EntityScriptJava::create(app, entity, name);
        };
    }

    Ref<EntityScript> createInstance(const std::string& name, const Ref<Application>& app, const Ref<Entity>& entity) const {
        try {
            return this->scripts.at(name)(app, entity);
        } catch (const std::out_of_range&) {
            throw std::runtime_error("Script not found: " + name);
        }
    }

private:
    std::unordered_map<std::string, std::function<Ref<EntityScript>(const Ref<Application>&, const Ref<Entity>&)>> scripts;
};
