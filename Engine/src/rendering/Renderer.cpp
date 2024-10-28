#include "pch/enginepch.h"
#include "Renderer.h"


void Renderer::setIrradianceSH(const std::array<glm::vec3, 9>& irradianceSh) {
    this->irradianceSH = irradianceSh;
}

void Renderer::setPrefilteredEnvMap(const Ref<TextureCube>& prefilteredEnvMap) {
    this->prefilteredEnvMap = prefilteredEnvMap;
}

void Renderer::setBRDFLUT(const Ref<Texture2D>& brdfLUT) {
    this->brdfLUT = brdfLUT;
}

void Renderer::setDirectionalLight(const Ref<DirectionalLight>& directionalLight) {
    this->directionalLight = directionalLight;
}

void Renderer::addPointLight(const PointLight& pointLight) {
    this->pointLights.push_back(pointLight);
}
