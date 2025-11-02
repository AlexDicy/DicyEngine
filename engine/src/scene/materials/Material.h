#pragma once

struct Material {
    Ref<Texture> albedo;
    Ref<Texture> occlusionRoughnessMetallic;
    bool ignoreLighting = false;

    Material() = default;
    Material(const Ref<Texture>& albedo, const Ref<Texture>& occlusionRoughnessMetallic) : albedo(albedo), occlusionRoughnessMetallic(occlusionRoughnessMetallic) {}
    Material(const Ref<Texture>& albedo) : albedo(albedo), occlusionRoughnessMetallic(nullptr) {}
};
