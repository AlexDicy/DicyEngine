#include "pch/enginepch.h"
#include "ModelImporter.h"

#include "images/ImageUtils.h"

#include <stack>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/texture.h>
#include <stb_image.h>

glm::mat4 convertAiMatrixToGlm(const aiMatrix4x4& aiMatrix) {
    return {
        aiMatrix.a1, aiMatrix.b1, aiMatrix.c1, aiMatrix.d1, //
        aiMatrix.a2, aiMatrix.b2, aiMatrix.c2, aiMatrix.d2, //
        aiMatrix.a3, aiMatrix.b3, aiMatrix.c3, aiMatrix.d3, //
        aiMatrix.a4, aiMatrix.b4, aiMatrix.c4, aiMatrix.d4, //
    };
}

std::vector<Model> ModelImporter::importFromFile(const Ref<Renderer>& renderer, const std::string& filename) {
    Assimp::Importer importer;

    // TODO useful flags: aiProcess_SplitLargeMeshes, aiProcess_OptimizeMeshes, aiProcess_GenNormals
    constexpr int flags =
        aiProcess_MakeLeftHanded | aiProcess_FlipWindingOrder | aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType;
    const aiScene* scene = importer.ReadFile(filename, flags);
    if (scene == nullptr) {
        throw std::runtime_error(importer.GetErrorString());
    }

    const std::string basePath = filename.substr(0, filename.find_last_of('/'));


    // TODO: cache the vertex data, indexes, and textures indexed by the mesh/texture index
    // std::map<int, std::
    std::vector<Model> models;
    models.reserve(scene->mNumMeshes);

    std::stack<std::pair<aiNode*, glm::mat4>> nodeStack;
    nodeStack.emplace(scene->mRootNode, glm::mat4(1.0f));

    while (!nodeStack.empty()) {
        auto [node, parentTransformation] = nodeStack.top();
        nodeStack.pop();

        glm::mat4 currentTransformation = parentTransformation * convertAiMatrixToGlm(node->mTransformation);

        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            const unsigned int meshIndex = node->mMeshes[i];
            const aiMesh* mesh = scene->mMeshes[meshIndex];
            Model model(filename, meshIndex);
            model.transformationMatrix = currentTransformation;

            const aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
            model.material.albedo = getTextureFromMaterial(renderer, scene, material, aiTextureType_BASE_COLOR, basePath);
            // might have to load the 3 channels separately depending on the file format
            model.material.occlusionRoughnessMetallic = getTextureFromMaterial(renderer, scene, material, aiTextureType_DIFFUSE_ROUGHNESS, basePath);

            model.vertices.reserve(mesh->mNumVertices);
            for (unsigned int v = 0; v < mesh->mNumVertices; v++) {
                const aiVector3D normal = mesh->mNormals == nullptr ? aiVector3D(0.0f, 1.0f, 0.0f) : mesh->mNormals[v];

                const glm::vec2 textureCoords = mesh->mTextureCoords[0] == nullptr ? glm::vec2() : glm::vec2(mesh->mTextureCoords[0][v].x, mesh->mTextureCoords[0][v].y);

                model.vertices.emplace_back( //
                    mesh->mVertices[v].x, //
                    mesh->mVertices[v].y, //
                    mesh->mVertices[v].z, //
                    glm::vec3(normal.x, normal.y, normal.z),
                    textureCoords //
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
            nodeStack.emplace(node->mChildren[i], currentTransformation);
        }
    }

    return models;
}

Ref<Texture> ModelImporter::getTextureFromMaterial(const Ref<Renderer>& renderer, const aiScene* scene, const aiMaterial* material, const aiTextureType type,
                                                   const std::string& basePath) {
    if (material->GetTextureCount(type) == 0) {
        aiColor3D diffuseColor = {1.0f, 1.0f, 1.0f};
        material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);
        const auto colorData = std::array{
            static_cast<unsigned char>(diffuseColor.r * 255),
            static_cast<unsigned char>(diffuseColor.g * 255),
            static_cast<unsigned char>(diffuseColor.b * 255),
            static_cast<unsigned char>(255),
        };
        return Texture::builder().width(1).height(1).format(TextureFormat::RGBA).internalFormat(TextureInternalFormat::RGBA8).data(colorData.data()).build(renderer);
    }

    aiString aiTexturePath;
    if (material->GetTexture(type, 0, &aiTexturePath) != AI_SUCCESS) {
        return nullptr;
    }

    const std::string texturePath = aiTexturePath.C_Str();
    if (texturePath[0] == '*') {
        const aiTexture* texture = scene->mTextures[std::stoi(texturePath.substr(1))];
        auto data = reinterpret_cast<unsigned char*>(texture->pcData);
        unsigned int channels = 4;
        unsigned int width = texture->mWidth;
        unsigned int height = texture->mHeight;
        if (texture->mHeight == 0) { // compressed texture
            data = decompressTexture(data, texture->mWidth, channels, width, height);
        }
        const TextureFormat format = channels == 4 ? TextureFormat::RGBA : TextureFormat::RGB;
        const TextureInternalFormat internalFormat = channels == 4 ? TextureInternalFormat::RGBA8 : TextureInternalFormat::RGB8;
        return Texture::builder().width(width).height(height).format(format).internalFormat(internalFormat).data(data).build(renderer);
    }
    return ImageUtils::loadTextureFromFile(renderer, basePath + "/" + texturePath);
}

unsigned char* ModelImporter::decompressTexture(const unsigned char* data, const unsigned int size, unsigned int& channels, unsigned int& width, unsigned int& height) {
    int stbiWidth, stbiHeight, stbiChannels; // not unsigned
    stbi_set_flip_vertically_on_load(true);
    stbi_uc* texture =
        stbi_load_from_memory(data, size, &stbiWidth, &stbiHeight, &stbiChannels, 0); // NOLINT(bugprone-narrowing-conversions, cppcoreguidelines-narrowing-conversions)

    channels = stbiChannels;
    width = stbiWidth;
    height = stbiHeight;

    if (texture) {
        return texture;
    }
    const char* error = stbi_failure_reason();
    DE_ERROR("Failed to decompress texture: {0}", error);
    return nullptr;
}
