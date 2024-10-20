#include "pch/enginepch.h"
#include "model_importer.h"

#include <stack>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/texture.h>
#include <stb_image.h>

glm::mat4 convert_ai_matrix_to_glm(const aiMatrix4x4& ai_matrix) {
    return {
        ai_matrix.a1, ai_matrix.b1, ai_matrix.c1, ai_matrix.d1, //
        ai_matrix.a2, ai_matrix.b2, ai_matrix.c2, ai_matrix.d2, //
        ai_matrix.a3, ai_matrix.b3, ai_matrix.c3, ai_matrix.d3, //
        ai_matrix.a4, ai_matrix.b4, ai_matrix.c4, ai_matrix.d4, //
    };
}

std::vector<Model> ModelImporter::import_from_file(const Ref<Renderer>& renderer, const std::string& filename) {
    Assimp::Importer importer;

    // TODO useful flags: aiProcess_SplitLargeMeshes, aiProcess_OptimizeMeshes, aiProcess_GenNormals
    constexpr int flags =
        aiProcess_MakeLeftHanded | aiProcess_FlipWindingOrder | aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType;
    const aiScene* scene = importer.ReadFile(filename, flags);
    if (scene == nullptr) {
        throw std::runtime_error(importer.GetErrorString());
    }

    std::string base_path = filename.substr(0, filename.find_last_of('/'));


    // TODO: cache the vertex data, indexes, and textures indexed by the mesh/texture index
    // std::map<int, std::
    std::vector<Model> models;
    models.reserve(scene->mNumMeshes);

    std::stack<std::pair<aiNode*, glm::mat4>> node_stack;
    node_stack.emplace(scene->mRootNode, glm::mat4(1.0f));

    while (!node_stack.empty()) {
        auto [node, parent_transformation] = node_stack.top();
        node_stack.pop();

        glm::mat4 current_transformation = parent_transformation * convert_ai_matrix_to_glm(node->mTransformation);

        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            const unsigned int mesh_index = node->mMeshes[i];
            const aiMesh* mesh = scene->mMeshes[mesh_index];
            Model model;
            model.transformation_matrix = current_transformation;

            const aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
            model.material.albedo = get_texture_from_material(renderer, scene, material, aiTextureType_BASE_COLOR, base_path);
            // might have to load the 3 channels separately depending on the file format
            model.material.occlusion_roughness_metallic = get_texture_from_material(renderer, scene, material, aiTextureType_DIFFUSE_ROUGHNESS, base_path);

            model.vertices.reserve(mesh->mNumVertices);
            for (unsigned int v = 0; v < mesh->mNumVertices; v++) {
                const aiVector3D normal = mesh->mNormals == nullptr ? aiVector3D(0.0f, 1.0f, 0.0f) : mesh->mNormals[v];

                const glm::vec2 texture_coords = mesh->mTextureCoords[0] == nullptr ? glm::vec2() : glm::vec2(mesh->mTextureCoords[0][v].x, mesh->mTextureCoords[0][v].y);

                model.vertices.emplace_back( //
                    mesh->mVertices[v].x, //
                    mesh->mVertices[v].y, //
                    mesh->mVertices[v].z, //
                    glm::vec3(normal.x, normal.y, normal.z),
                    texture_coords //
                );
            }

            model.indexes.reserve(static_cast<size_t>(mesh->mNumFaces) * 3);
            for (unsigned int f = 0; f < mesh->mNumFaces; f++) {
                model.indexes.push_back(mesh->mFaces[f].mIndices[0]);
                model.indexes.push_back(mesh->mFaces[f].mIndices[1]);
                model.indexes.push_back(mesh->mFaces[f].mIndices[2]);
            }

            models.push_back(model);
        }

        for (unsigned int i = 0; i < node->mNumChildren; i++) {
            node_stack.emplace(node->mChildren[i], current_transformation);
        }
    }

    return models;
}

Ref<Texture2D> ModelImporter::get_texture_from_material(const Ref<Renderer>& renderer, const aiScene* scene, const aiMaterial* material, const aiTextureType type,
                                                        const std::string& base_path) {
    if (material->GetTextureCount(type) == 0) {
        aiColor3D diffuse_color = {1.0f, 1.0f, 1.0f};
        material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse_color);
        const auto color_data = std::array{
            static_cast<unsigned char>(diffuse_color.r * 255),
            static_cast<unsigned char>(diffuse_color.g * 255),
            static_cast<unsigned char>(diffuse_color.b * 255),
            static_cast<unsigned char>(255),
        };
        return renderer->create_texture2d(4, 1, 1, 1, color_data.data());
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
        return renderer->create_texture2d(channels, width, height, 1, data);
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
