#include "pch/enginepch.h"
#include "renderer.h"

void Renderer::prepare_ambient_light(const Ref<TextureCube>& irradiance_map) {
    this->irradiance_map = irradiance_map;
}

