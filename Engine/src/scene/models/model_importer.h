#pragma once
#include "model.h"
#include "scene/entities/entity.h"

#include <assimp/material.h>


struct aiScene;

class ModelImporter {
public:
    static std::vector<Model> import_from_file(const Ref<Renderer>& renderer, const std::string& filename);

private:
    static Ref<Texture2D> get_texture_from_material(const Ref<Renderer>& renderer, const aiScene* scene, const aiMaterial* material, aiTextureType type,
                                                    const std::string& base_path);
    static unsigned char* decompress_texture(const unsigned char* data, unsigned int size, unsigned int& channels, unsigned int& width, unsigned int& height);
};
