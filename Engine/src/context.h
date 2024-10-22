// ReSharper disable CppClangTidyCppcoreguidelinesAvoidConstOrRefDataMembers
#pragma once
#include "rendering/renderer.h"

class Context {
    friend class Application;

public:
    const Ref<Renderer>& renderer;
    const float& deltaTime = _deltaTime;

    explicit Context(const Ref<Renderer>& renderer) : renderer(renderer) {}

protected:
    void setDeltaTime(const float& deltaTime) {
        _deltaTime = deltaTime;
    }

private:
    // ReSharper disable once CppInconsistentNaming
    float _deltaTime = 0;
};
