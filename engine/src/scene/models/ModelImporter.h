#pragma once
#include "Model.h"
#include "scene/entities/Entity.h"

#include <assimp/material.h>


struct aiScene; // NOLINT, no need to include assimp

class ModelImporter {
public:
    static std::vector<Model> importFromFile(const Ref<Renderer>& renderer, const std::string& filename);

private:
    static Ref<Texture> getTextureFromMaterial(const Ref<Renderer>& renderer, const aiScene* scene, const aiMaterial* material, aiTextureType type,
                                                    const std::string& basePath);
    static unsigned char* decompressTexture(const unsigned char* data, unsigned int size, unsigned int& channels, unsigned int& width, unsigned int& height);
};
