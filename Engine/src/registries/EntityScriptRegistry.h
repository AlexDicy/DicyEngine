#pragma once
#include "scene/entities/EntityScript.h"
#include "scene/entities/EntityScriptJava.h"

class EntityScriptRegistry {
public:
    EntityScriptRegistry() = default;

    template <typename T>
    void registerScript(const std::string& name, EntityScript::ScriptType type) {
        if (type == EntityScript::ScriptType::NATIVE) {
            this->scripts[name] = [](const Ref<Application>& app, const Ref<Entity>& entity) {
                return std::make_shared<T>(app, entity);
            };
            return;
        }

        if (type == EntityScript::ScriptType::JVM) {
            this->scripts[name] = [name](const Ref<Application>& app, const Ref<Entity>& entity) {
                return EntityScriptJava::create(app, entity, name);
            };
            return;
        }

        DE_ERROR("Script type not expected");
    }

    Ref<EntityScript> createInstance(const std::string& name, const Ref<Application>& app, const Ref<Entity>& entity) const {
        try {
            return this->scripts.at(name)(app, entity);
        } catch (const std::out_of_range&) {
            DE_ERROR("Script not found: {0}", name);
            throw;
        }
    }

private:
    std::unordered_map<std::string, std::function<Ref<EntityScript>(const Ref<Application>&, const Ref<Entity>&)>> scripts;
};
