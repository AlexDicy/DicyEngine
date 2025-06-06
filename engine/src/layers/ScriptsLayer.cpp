#include "pch/enginepch.h"
#include "ScriptsLayer.h"

void ScriptsLayer::update(const std::unique_ptr<Context>& ctx) {
    DE_PROFILE_FUNCTION();
    const auto scriptsView = this->scene->getScripts();
    for (const auto& entity : scriptsView) {
        Script& script = scriptsView.get<Script>(entity);
        script.getEntityScript()->onUpdate(ctx->deltaTime);
    }
}
