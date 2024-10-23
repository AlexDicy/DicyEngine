#include "pch/enginepch.h"
#include "renderer.h"


void Renderer::setIrradianceSH(const std::array<glm::vec3, 9>& irradianceSh)  {
    this->irradianceSH = irradianceSh;
}
