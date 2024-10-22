#pragma once

struct Material {
    Ref<Texture2D> albedo;
    Ref<Texture2D> occlusionRoughnessMetallic;
    bool ignoreLighting = false;

    Material() = default;
    Material(const Ref<Texture2D>& albedo, const Ref<Texture2D>& occlusionRoughnessMetallic) : albedo(albedo), occlusionRoughnessMetallic(occlusionRoughnessMetallic) {}
    Material(const Ref<Texture2D>& albedo) : albedo(albedo), occlusionRoughnessMetallic(nullptr) {}
};
