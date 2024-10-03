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

    std::string base_path = filename.substr(0, filename.find_last_of('/'));

    aiMatrix4x4 ai_tm = scene->mRootNode->mTransformation;
    glm::mat4 root_transformation = glm::mat4(ai_tm.a1, ai_tm.b1, ai_tm.c1, ai_tm.d1, //
                                              ai_tm.a2, ai_tm.b2, ai_tm.c2, ai_tm.d2, //
                                              ai_tm.a3, ai_tm.b3, ai_tm.c3, ai_tm.d3, //
                                              ai_tm.a4, ai_tm.b4, ai_tm.c4, ai_tm.d4);

    std::vector<Model> models(scene->mNumMeshes);
    for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
        Model* model = &models[i];
        model->transformation_matrix = root_transformation;
        aiMesh* mesh = scene->mMeshes[i];

        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        model->material.albedo = get_texture_from_material(renderer, scene, material, aiTextureType_BASE_COLOR, base_path);
        // might have to load the 3 channels separately depending on the file format
        model->material.occlusion_roughness_metallic = get_texture_from_material(renderer, scene, material, aiTextureType_DIFFUSE_ROUGHNESS, base_path);

        models[i].vertices.reserve(mesh->mNumVertices);
        for (unsigned int v = 0; v < mesh->mNumVertices; v++) {
            aiVector3f normal = mesh->mNormals == nullptr ? aiVector3D(0.0f, 1.0f, 0.0f) : mesh->mNormals[v];

            const glm::vec2 texture_coords = mesh->mTextureCoords[0] == nullptr ? glm::vec2() : glm::vec2(mesh->mTextureCoords[0][v].x, mesh->mTextureCoords[0][v].y);

            models[i].vertices.emplace_back( //
                mesh->mVertices[v].x, //
                mesh->mVertices[v].y, //
                mesh->mVertices[v].z, //
                glm::vec3(normal.x, normal.y, normal.z),
                texture_coords //
            );
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

Ref<Texture2D> ModelImporter::get_texture_from_material(const Ref<Renderer>& renderer, const aiScene* scene, const aiMaterial* material, const aiTextureType type,
                                                        const std::string& base_path) {
    if (material->GetTextureCount(type) == 0) {
        return nullptr;
    }

    aiString ai_texture_path;
    if (material->GetTexture(type, 0, &ai_texture_path) != AI_SUCCESS) {
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
    return renderer->create_texture2d(base_path + "/" + texture_path);
}

unsigned char* ModelImporter::decompress_texture(const unsigned char* data, const unsigned int size, unsigned int& channels, unsigned int& width, unsigned int& height) {
    int stbi_width, stbi_height, stbi_channels;
    stbi_uc* texture =
        stbi_load_from_memory(data, size, &stbi_width, &stbi_height, &stbi_channels, 0); // NOLINT(bugprone-narrowing-conversions, cppcoreguidelines-narrowing-conversions)

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
