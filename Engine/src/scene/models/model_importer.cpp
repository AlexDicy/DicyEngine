#include "pch/enginepch.h"
#include "model_importer.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


Model ModelImporter::import_from_file(const std::string& filename) {
    DE_INFO("Importing model from file \"{0}\"", filename.c_str());
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(filename, aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);
    if (scene == nullptr) {
        throw std::exception(importer.GetErrorString());
    }

    DE_INFO("\tn. of meshes {0}", scene->mNumMeshes);

    aiNode* node = scene->mRootNode;
    return {};
}
