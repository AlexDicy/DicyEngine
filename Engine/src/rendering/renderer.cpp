#include "pch/enginepch.h"
#include "renderer.h"


void Renderer::set_irradiance_sh(const std::array<glm::vec3, 9>& irradiance_sh)  {
    this->irradiance_sh = irradiance_sh;
}
