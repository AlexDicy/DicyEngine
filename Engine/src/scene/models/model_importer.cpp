#include "pch/enginepch.h"
#include "model_importer.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


std::vector<Model> ModelImporter::import_from_file(const std::string& filename) {
    Assimp::Importer importer;

    // TODO: aiProcess_MakeLeftHanded is probably needed, others: aiProcess_SplitLargeMeshes, aiProcess_OptimizeMeshes
    const aiScene* scene = importer.ReadFile(filename, aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);
    if (scene == nullptr) {
        throw std::runtime_error(importer.GetErrorString());
    }

    std::vector<Model> models(scene->mNumMeshes);
    for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[i];

        models[i].vertices.reserve(mesh->mNumVertices);
        for (unsigned int v = 0; v < mesh->mNumVertices; v++) {
            models[i].vertices.push_back({
                mesh->mVertices[v].x,
                mesh->mVertices[v].y,
                mesh->mVertices[v].z,
                static_cast<float>(glm::cos(v)),
                1.0f - (static_cast<float>(v) / static_cast<float>(mesh->mNumVertices)),
                (static_cast<float>(v) / static_cast<float>(mesh->mNumVertices)),
                1.0f,
            });
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
