// ReSharper disable CppClangTidyCppcoreguidelinesAvoidConstOrRefDataMembers
#pragma once
#include "Application.h"

class Context {
    friend class Application;

public:
    const Ref<Application>& app;
    const Ref<Renderer>& renderer;

    const float& deltaTime = _deltaTime;

    explicit Context(const Ref<Application>& app) : app(app), renderer(app->getRenderer()) {}

protected:
    void setDeltaTime(const float& deltaTime) {
        _deltaTime = deltaTime;
    }

private:
    // ReSharper disable once CppInconsistentNaming
    float _deltaTime = 0;
};
