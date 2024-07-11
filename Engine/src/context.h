// ReSharper disable CppClangTidyCppcoreguidelinesAvoidConstOrRefDataMembers
#pragma once
#include "rendering/renderer.h"

class Context {
    friend class Application;

public:
    const Ref<Renderer>& renderer;
    const float& delta_time = _delta_time;

    explicit Context(const Ref<Renderer>& renderer) : renderer(renderer) {}

protected:
    void set_delta_time(const float& delta_time) {
        _delta_time = delta_time;
    }

private:
    float _delta_time = 0;
};
