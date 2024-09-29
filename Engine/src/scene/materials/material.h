#pragma once

struct Material {
    Ref<Texture2D> albedo;
    Ref<Texture2D> roughness;
    Ref<Texture2D> metallic;

    Material() = default;
    Material(const Ref<Texture2D>& albedo, const Ref<Texture2D>& roughness, const Ref<Texture2D>& metallic) : albedo(albedo), roughness(roughness), metallic(metallic) {}
    Material(const Ref<Texture2D>& albedo) : albedo(albedo), roughness(nullptr), metallic(nullptr) {}
};
