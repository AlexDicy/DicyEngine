#include "pch/enginepch.h"
#include "model_importer.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


std::vector<Model> ModelImporter::import_from_file(const std::string& filename) {
    Assimp::Importer importer;

    // TODO: aiProcess_MakeLeftHanded is probably needed, others: aiProcess_SplitLargeMeshes, aiProcess_OptimizeMeshes, aiProcess_GenNormals
    const aiScene* scene = importer.ReadFile(filename, aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);
    if (scene == nullptr) {
        throw std::runtime_error(importer.GetErrorString());
    }

    std::vector<Model> models(scene->mNumMeshes);
    for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[i];

        models[i].vertices.reserve(mesh->mNumVertices);
        for (unsigned int v = 0; v < mesh->mNumVertices; v++) {
            aiVector3f normal = mesh->mNormals == nullptr ? aiVector3D(0.0f, 1.0f, 0.0f) : mesh->mNormals[v];

            models[i].vertices.push_back({
                mesh->mVertices[v].x,
                mesh->mVertices[v].y,
                mesh->mVertices[v].z,
                0.7f,
                0.7f,
                0.9f,
                1.0f,
                {normal.x, normal.y, normal.z}
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
