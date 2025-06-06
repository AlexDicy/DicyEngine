// ReSharper disable CppClangTidyCppcoreguidelinesAvoidConstOrRefDataMembers
#pragma once
#include "Application.h"

class Context {
    friend class Application;

public:
    const Ref<Application>& app;
    const Ref<Physics>& physics;
    const Ref<Renderer>& renderer;
    const Ref<Scene>& scene;

    const float& deltaTime = _deltaTime;

    explicit Context(const Ref<Application>& app, const Ref<Scene>& scene) : app(app), physics(app->getPhysics()), renderer(app->getRenderer()), scene(scene) {}

protected:
    void setDeltaTime(const float& deltaTime) {
        _deltaTime = deltaTime;
    }

private:
    // ReSharper disable once CppInconsistentNaming
    float _deltaTime = 0;
};
