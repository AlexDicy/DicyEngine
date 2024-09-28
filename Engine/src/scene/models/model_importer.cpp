#include "pch/enginepch.h"
#include "model_importer.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/texture.h>
#include <stb_image.h>


std::vector<Model> ModelImporter::import_from_file(const Ref<Renderer>& renderer, const std::string& filename) {
    Assimp::Importer importer;

    // TODO: aiProcess_MakeLeftHanded is probably needed, others: aiProcess_SplitLargeMeshes, aiProcess_OptimizeMeshes, aiProcess_GenNormals
    const aiScene* scene = importer.ReadFile(filename, aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);
    if (scene == nullptr) {
        throw std::runtime_error(importer.GetErrorString());
    }

    std::vector<Model> models(scene->mNumMeshes);
    for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[i];

        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        aiColor3D diffuse_color = {1.0f, 1.0f, 1.0f};
        material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse_color);
        models[i].texture = get_texture_from_material(renderer, scene, material);

        models[i].vertices.reserve(mesh->mNumVertices);
        for (unsigned int v = 0; v < mesh->mNumVertices; v++) {
            aiVector3f normal = mesh->mNormals == nullptr ? aiVector3D(0.0f, 1.0f, 0.0f) : mesh->mNormals[v];

            models[i].vertices.push_back(
                {mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z, diffuse_color.r, diffuse_color.g, diffuse_color.b, 1.0f, {normal.x, normal.y, normal.z}});
        }

        models[i].indexes.reserve(static_cast<size_t>(mesh->mNumFaces) * 3);
        for (unsigned int f = 0; f < mesh->mNumFaces; f++) {
            models[i].indexes.push_back(mesh->mFaces[f].mIndices[0]);
            models[i].indexes.push_back(mesh->mFaces[f].mIndices[1]);
            models[i].indexes.push_back(mesh->mFaces[f].mIndices[2]);
        }
    }

    return models;
}

Ref<Texture2D> ModelImporter::get_texture_from_material(const Ref<Renderer>& renderer, const aiScene* scene, const aiMaterial* material) {
    if (material->GetTextureCount(aiTextureType_DIFFUSE) == 0) {
        return nullptr;
    }

    aiString ai_texture_path;
    if (material->GetTexture(aiTextureType_DIFFUSE, 0, &ai_texture_path) != AI_SUCCESS) {
        return nullptr;
    }

    std::string texture_path = ai_texture_path.C_Str();
    if (texture_path[0] == '*') {
        aiTexture* texture = scene->mTextures[std::stoi(texture_path.substr(1))];
        auto data = reinterpret_cast<unsigned char*>(texture->pcData);
        unsigned int channels = 4;
        unsigned int width = texture->mWidth;
        unsigned int height = texture->mHeight;
        if (texture->mHeight == 0) { // compressed texture
            data = decompress_texture(data, texture->mWidth, channels, width, height);
        }
        return renderer->create_texture2d(channels, width, height, data);
    }
    return renderer->create_texture2d(texture_path);
}

unsigned char* ModelImporter::decompress_texture(const unsigned char* data, const unsigned int size, unsigned int& channels, unsigned int& width, unsigned int& height) {
    int stbi_width, stbi_height, stbi_channels;
    stbi_uc* texture = stbi_load_from_memory(data, size, &stbi_width, &stbi_height, &stbi_channels, 0);  // NOLINT(bugprone-narrowing-conversions, cppcoreguidelines-narrowing-conversions)

    channels = stbi_channels;
    width = stbi_width;
    height = stbi_height;

    if (texture) {
        return texture;
    }
    const char* error = stbi_failure_reason();
    DE_ERROR("Failed to decompress texture: {0}", error);
    return nullptr;
}
