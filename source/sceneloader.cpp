#include "sceneloader.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>
#include <fmt/format.h>
#include <map>
#include <filesystem>
#include <memory>

Scene SceneLoader::loadScene(const std::string_view &fileName)
{
    auto processMaterial = [&](aiMaterial *aiMaterial) -> material_t {
        std::cout << fmt::format("Material \"{}\"\n", aiMaterial->GetName().C_Str());

        auto colorStr = [&](const aiColor3D &color) {
            return fmt::format("{} {} {}", color.r, color.g, color.b);
        };

        auto aiMatColorDiffuse = aiColor3D{};
        aiMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, aiMatColorDiffuse);
        auto aiMatColorSpecular = aiColor3D{};
        aiMaterial->Get(AI_MATKEY_COLOR_SPECULAR, aiMatColorSpecular);
        auto aiMatColorAmbient = aiColor3D{};
        aiMaterial->Get(AI_MATKEY_COLOR_AMBIENT, aiMatColorAmbient);
        auto aiMatColorEmissive = aiColor3D{};
        aiMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, aiMatColorEmissive);
        auto aiMatColorTransparent = aiColor3D{};
        aiMaterial->Get(AI_MATKEY_COLOR_TRANSPARENT, aiMatColorTransparent);
        auto aiMatColorReflective = aiColor3D{};
        aiMaterial->Get(AI_MATKEY_COLOR_REFLECTIVE, aiMatColorReflective);

        std::cout << fmt::format("\tdiffuse: {}\n", colorStr(aiMatColorDiffuse))
                  << fmt::format("\tspecular: {}\n", colorStr(aiMatColorSpecular))
                  << fmt::format("\tambient: {}\n", colorStr(aiMatColorAmbient))
                  << fmt::format("\temissive: {}\n", colorStr(aiMatColorEmissive))
                  << fmt::format("\ttransparent: {}\n", colorStr(aiMatColorTransparent))
                  << fmt::format("\treflective: {}\n", colorStr(aiMatColorReflective));

        float aiReflectivity = std::numeric_limits<float>::infinity();
        aiMaterial->Get(AI_MATKEY_REFLECTIVITY, aiReflectivity);

        float aiOpacity = std::numeric_limits<float>::infinity();
        aiMaterial->Get(AI_MATKEY_OPACITY, aiOpacity);

        float aiShininess = std::numeric_limits<float>::infinity();
        aiMaterial->Get(AI_MATKEY_SHININESS, aiShininess);

        float aiRefracti = std::numeric_limits<float>::infinity();
        aiMaterial->Get(AI_MATKEY_REFRACTI, aiRefracti);

        std::cout << fmt::format("\treflectivity: {}\n", aiReflectivity)
                  << fmt::format("\topacity: {}\n", aiOpacity)
                  << fmt::format("\tshininess: {}\n", aiShininess)
                  << fmt::format("\tIoR: {}\n", aiRefracti);

        std::map<int, std::string> aiShadingNames;
        aiShadingNames[-1] = "unknown";
#define addShadingStr(value)\
        aiShadingNames[value] = #value

        addShadingStr(aiShadingMode_Flat);
        addShadingStr(aiShadingMode_Gouraud);
        addShadingStr(aiShadingMode_Phong);
        addShadingStr(aiShadingMode_Blinn);
        addShadingStr(aiShadingMode_Toon);
        addShadingStr(aiShadingMode_OrenNayar);
        addShadingStr(aiShadingMode_Minnaert);
        addShadingStr(aiShadingMode_CookTorrance);
        addShadingStr(aiShadingMode_NoShading);
        addShadingStr(aiShadingMode_Fresnel);
#undef addShadingStr

        int aiShadingMode = -1;
        aiMaterial->Get(AI_MATKEY_SHADING_MODEL, aiShadingMode);

        std::cout << fmt::format("\tshading: {}\n", aiShadingNames[aiShadingMode]);

        if(aiShadingMode == aiShadingMode_Blinn) {
            if(aiMatColorEmissive != aiColor3D{}) {
                std::cout << fmt::format("\t\tlight: {}\n", colorStr(aiMatColorEmissive));
                const auto emission = vec3_t{
                    aiMatColorEmissive.r, aiMatColorEmissive.g, aiMatColorEmissive.b
                };
                return material_t::createLight(emission);
            } else {
                if(aiReflectivity != 0.0) {
                    std::cout << fmt::format("\t\tspecular: {}\n", colorStr(aiMatColorReflective));
                    const auto diffuse = vec3_t{
                        aiMatColorReflective.r, aiMatColorReflective.g, aiMatColorReflective.b
                    };
                    return material_t::createSpecular(diffuse);
                } else {
                    std::cout << fmt::format("\t\tdiffuse: {}\n", colorStr(aiMatColorDiffuse));
                    const auto diffuse = vec3_t{
                        aiMatColorDiffuse.r, aiMatColorDiffuse.g, aiMatColorDiffuse.b
                    };
                    return material_t::createDiffuse(diffuse);
                }
            }
        } else if(aiShadingMode == aiShadingMode_Gouraud) {
            std::cout << fmt::format("\t\trefractive (ior = {}): {}\n", aiRefracti, colorStr(aiMatColorDiffuse));
            const auto diffuse = vec3_t{
                aiMatColorDiffuse.r, aiMatColorDiffuse.g, aiMatColorDiffuse.b
            };
            return material_t::createRefractive(diffuse, aiRefracti);
        } else {
            throw std::runtime_error("material error");
        }
        throw std::runtime_error("material error");
    };

    auto scene = Scene();

    auto importer = Assimp::Importer{};
    auto aiScene = importer.ReadFile(fileName.data(), aiProcess_Triangulate);
    if(nullptr == aiScene) {
        throw std::runtime_error(importer.GetErrorString());
    }

    scene.camera = deduceCamera(aiScene);

    auto processMesh = [&](unsigned int meshId, const aiMatrix4x4& transform) {
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
            const auto aiVertPos = transform * aiMesh->mVertices[vertId];
            positions[vertId] = vec3_t{
                aiVertPos.x,
                aiVertPos.y,
                aiVertPos.z
            };
            if(nullptr != aiMesh->mTextureCoords[0]) {
                texCoords[vertId] = TextureTriangle::tex_coord_t{
                    aiMesh->mTextureCoords[0][vertId].x,
                    aiMesh->mTextureCoords[0][vertId].y
                };
            }
        }

        const auto aiMaterial = aiScene->mMaterials[aiMesh->mMaterialIndex];
        auto material = material_t{};
        auto texture = std::shared_ptr<Texture>{ nullptr };
        if(aiMaterial->GetTextureCount(aiTextureType_DIFFUSE)) {
            auto textureName = aiString{};
            aiMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &textureName);
            auto sourcePath = std::filesystem::path(fileName).remove_filename();
            auto texturePath = std::filesystem::path(sourcePath.generic_string() + textureName.C_Str());
            texture = std::make_shared<Texture>(Texture::load(texturePath.generic_string()));
        } else {
            material = processMaterial(aiMaterial);
        }
        scene.addObject<Mesh>(faces, positions, texCoords, material, texture);
    };

    std::function<void(aiNode *)> processNode = [&](aiNode *node) {
        const auto aiNodeTransform = node->mTransformation;
        for(int i = 0; i < node->mNumMeshes; ++i) {
            processMesh(node->mMeshes[i], aiNodeTransform);
        }
        for(int i = 0; i < node->mNumChildren; ++i) {
            processNode(node->mChildren[i]);
        }
    };

    processNode(aiScene->mRootNode);

    return scene;
}

Camera SceneLoader::deduceCamera(const aiScene *aiScene)
{
    if(aiScene->mNumCameras != 1) {
        throw std::runtime_error("Camera is arbitrary");
    }

    const auto aiCamera = aiScene->mCameras[0];
    const auto aiCameraNode = aiScene->mRootNode->FindNode(aiCamera->mName);
    if(aiCameraNode == nullptr) {
        throw std::runtime_error("Camera transform is unknown");
    }
    const auto aiCameraTransform = aiCameraNode->mTransformation;

    const auto aiCameraPosition = aiCameraTransform * aiCamera->mPosition;
    const auto cameraPosition = vec3_t{ aiCameraPosition.x, aiCameraPosition.y, aiCameraPosition.z };

    const auto aiCameraTransformRotation = aiMatrix3x3(aiCameraTransform);

    const auto aiCameraLookAt = aiCameraTransformRotation * aiCamera->mLookAt;
    const auto aiCameraGaze = aiCameraLookAt;
    const auto cameraGaze = vec3_t{ aiCameraGaze.x, aiCameraGaze.y, aiCameraGaze.z };

    const auto aiCameraUp = aiCameraTransformRotation * aiCamera->mUp;
    const auto cameraUp = vec3_t{ aiCameraUp.x, aiCameraUp.y, aiCameraUp.z };

    return Camera(cameraPosition, cameraGaze, 50.0, cameraUp);
}
