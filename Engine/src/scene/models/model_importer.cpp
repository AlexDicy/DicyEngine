#include "pch/enginepch.h"
#include "model_importer.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

struct Vertex {
    float x, y, z;
    float r, g, b, a;
};

struct TempMeshStruct {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indexes;
};

Model ModelImporter::import_from_file(const std::string& filename, const Ref<Entity>& temp_entity, const Ref<Renderer>& renderer) {
    DE_INFO("Importing model from file \"{0}\"", filename.c_str());
    Assimp::Importer importer;

    // TODO: aiProcess_MakeLeftHanded is probably needed, others: aiProcess_SplitLargeMeshes, aiProcess_OptimizeMeshes
    const aiScene* scene = importer.ReadFile(filename, aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);
    if (scene == nullptr) {
        throw std::runtime_error(importer.GetErrorString());
    }

    DE_INFO("\tn. of meshes {0}", scene->mNumMeshes);
    DE_INFO("\tn. of materials {0}", scene->mNumMaterials);
    DE_INFO("\tn. of textures {0}", scene->mNumTextures);

    std::vector<TempMeshStruct> meshes(scene->mNumMeshes);
    for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[i];
        DE_INFO("\tMesh: {0}", mesh->mName.C_Str());
        DE_INFO("\t\tn. of vertices: {0}", mesh->mNumVertices);
        DE_INFO("\t\tn. of faces: {0}", mesh->mNumFaces);

        meshes[i].vertices.reserve(mesh->mNumVertices);
        for (unsigned int v = 0; v < mesh->mNumVertices; v++) {
            meshes[i].vertices.push_back({
                mesh->mVertices[v].x,
                mesh->mVertices[v].y,
                mesh->mVertices[v].z,
                static_cast<float>(glm::cos(v)),
                1.0f - (static_cast<float>(v) / static_cast<float>(mesh->mNumVertices)),
                (static_cast<float>(v) / static_cast<float>(mesh->mNumVertices)),
                1.0f,
            });
        }

        meshes[i].indexes.reserve(mesh->mNumFaces * 3);
        for (unsigned int f = 0; f < mesh->mNumFaces; f++) {
            meshes[i].indexes.push_back(mesh->mFaces[f].mIndices[0]);
            meshes[i].indexes.push_back(mesh->mFaces[f].mIndices[1]);
            meshes[i].indexes.push_back(mesh->mFaces[f].mIndices[2]);
        }
    }

    float* vertex_data = reinterpret_cast<float*>(meshes[0].vertices.data());
    temp_entity->add<Mesh>(renderer, vertex_data, meshes[0].vertices.size() * sizeof(Vertex), meshes[0].indexes.data(), meshes[0].indexes.size());
    temp_entity->add<Transform>(glm::vec3({0.0f, 0.0f, 4.0f}), Rotation(), glm::vec3(2.46f));

    aiNode* node = scene->mRootNode;
    return {};
}
