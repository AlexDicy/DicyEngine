#include "pch/enginepch.h"
#include "Renderer.h"


const Ref<DepthFramebuffer>& Renderer::getShadowDepthFramebuffer() const {
    return this->shadowDepthFramebuffer;
}

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
    this->directionalLightViewProjection = directionalLight->getViewProjectionMatrix();
}

void Renderer::addPointLight(const PointLight& pointLight) {
    this->pointLights.push_back(pointLight);
}

void Renderer::setDirectionalShadowMapShader(const Ref<Shader>& shadowMapShader) {
    this->shadowMapShader = shadowMapShader;
}

void Renderer::setPointLightShadowMapShader(const Ref<Shader>& shader) {
    this->shadowCubeArrayShader = shader;
}
