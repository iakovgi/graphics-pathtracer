#include "sceneloader.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>
#include <fmt/format.h>

Scene SceneLoader::loadScene(const std::string_view &fileName)
{
    auto scene = Scene();

    auto importer = Assimp::Importer{};
    auto aiScene = importer.ReadFile(fileName.data(), aiProcess_Triangulate);
    if(nullptr == aiScene) {
        throw std::runtime_error(importer.GetErrorString());
    }

    for(int meshId = 0; meshId < aiScene->mNumMeshes; ++meshId) {
        const auto aiMesh = aiScene->mMeshes[meshId];

        std::vector<Mesh::face_t> faces(aiMesh->mNumFaces);
        for(int faceId = 0; faceId < aiMesh->mNumFaces; ++faceId) {
            faces[faceId] = Mesh::face_t{
                aiMesh->mFaces[faceId].mIndices[0],
                aiMesh->mFaces[faceId].mIndices[1],
                aiMesh->mFaces[faceId].mIndices[2]
            };
        }

        std::vector<vec3_t> positions(aiMesh->mNumVertices);
        std::vector<TextureTriangle::tex_coord_t> texCoords(aiMesh->mNumVertices);
        for(int vertId = 0; vertId < aiMesh->mNumVertices; ++vertId) {
            positions[vertId] = vec3_t{
                aiMesh->mVertices[vertId].x,
                aiMesh->mVertices[vertId].y,
                aiMesh->mVertices[vertId].z
            };
            if(nullptr != aiMesh->mTextureCoords[0]) {
                texCoords[vertId] = TextureTriangle::tex_coord_t{
                    aiMesh->mTextureCoords[0][vertId].x,
                    aiMesh->mTextureCoords[0][vertId].y
                };
            }
        }

        const auto aiMaterial = aiScene->mMaterials[aiMesh->mMaterialIndex];
        std::cout << fmt::format("mesh [{}]: material {}\n", meshId, aiMaterial->GetName().C_Str());
        auto aiDiffuse = aiColor3D{};
        aiMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, aiDiffuse);
        const auto diffuse = vec3_t{
            aiDiffuse.r, aiDiffuse.g, aiDiffuse.b
        };

        auto aiEmissive = aiColor3D{};
        aiMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, aiEmissive);
        const auto emissive = vec3_t{
            aiEmissive.r, aiEmissive.g, aiEmissive.b
        };

        auto material = material_t{};
        if(PTMATH_EPSILON >= emissive.lenSquared()) {
            material = material_t::createLight(emissive);
        } else {
            material = material_t::createDiffuse(diffuse);
        }

        scene.addObject<Mesh>(faces, positions, texCoords, material);
    }

    return scene;
}
