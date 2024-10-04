#pragma once

struct Material {
    Ref<Texture2D> albedo;
    Ref<Texture2D> occlusion_roughness_metallic;

    Material() = default;
    Material(const Ref<Texture2D>& albedo, const Ref<Texture2D>& occlusion_roughness_metallic) : albedo(albedo), occlusion_roughness_metallic(occlusion_roughness_metallic) {}
    Material(const Ref<Texture2D>& albedo) : albedo(albedo), occlusion_roughness_metallic(nullptr) {}
};
