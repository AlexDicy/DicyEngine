#include "pch/enginepch.h"
#include "SceneLayer.h"

#include <filesystem>
#include <glm/ext/matrix_transform.hpp>

#include "editor/scripts/CameraScript.h"
#include "editor/scripts/LightScript.h"
#include "images/Image.h"
#include "images/ImageUtils.h"
#include "images/LinearImage.h"
#include "rendering/skybox/SphericalHarmonics.h"
#include "scene/models/ModelImporter.h"

SceneLayer::SceneLayer(const Ref<Application>& app) {
    const Ref<Renderer>& renderer = app->getRenderer();
    this->scene = std::make_shared<Scene>();

    // x,y,z indicator
    std::vector<Model> xyzModels = ModelImporter::importFromFile(renderer, "../assets/models/arrows.glb");
    for (Model& arrow : xyzModels) {
        arrow.material.ignoreLighting = true;
    }
    this->addEntitiesForModels(renderer, xyzModels, {0.0f, 0.0f, 0.0f});

    // camera
    this->cameraEntity = this->scene->createEntity();
    this->cameraEntity->add<Transform>(glm::vec3(0.0f, 2.0f, 0.0f), Rotation(-15, 0, 0));
    this->cameraEntity->add<Script>(std::make_shared<CameraScript>(app, this->cameraEntity));

    this->shader = app->getShaderRegistry()->load("../assets/shaders/default-shader");
    Ref<Shader> skyboxShader = app->getShaderRegistry()->load("../assets/shaders/skybox-shader");
    Ref<LinearImage> skyboxHDR = std::make_shared<LinearImage>("../assets/skybox/kloofendal_48d_partly_cloudy_puresky_8k.hdr");
    Ref<LinearImage> skyboxToneMapped = ImageUtils::acesFilmicTonemapping(skyboxHDR);
    Ref<Texture2D> skyboxTexture =
        renderer->createTexture2D(skyboxHDR->getChannels(), skyboxHDR->getWidth(), skyboxHDR->getHeight(), skyboxHDR->getBytesPerPixel(), skyboxHDR->getData());
    Ref<Texture2D> skyboxTextureToneMapped = renderer->createTexture2D(skyboxToneMapped->getChannels(), skyboxToneMapped->getWidth(), skyboxToneMapped->getHeight(),
                                                                       skyboxToneMapped->getBytesPerPixel(), skyboxToneMapped->getData());
    Ref<Shader> equirectangularToCubemapShader = app->getShaderRegistry()->load("../assets/shaders/equirectangular-to-cubemap");
    Ref<TextureCube> skyboxCubeTexture = renderer->createTextureCubeFromHDR(skyboxTexture, equirectangularToCubemapShader, 2048);
    Ref<TextureCube> skyboxCubeToneMapped = renderer->createTextureCubeFromHDR(skyboxTextureToneMapped, equirectangularToCubemapShader, 256);
    Ref<CubeMap> skyboxCubeMap = skyboxCubeToneMapped->toCubemap();
    std::vector<glm::vec3> sphericalHarmonics = SphericalHarmonics::calculateSH(skyboxCubeMap, 3, true, true);
    renderer->setIrradianceSH({
        sphericalHarmonics[0],
        sphericalHarmonics[1],
        sphericalHarmonics[2],
        sphericalHarmonics[3],
        sphericalHarmonics[4],
        sphericalHarmonics[5],
        sphericalHarmonics[6],
        sphericalHarmonics[7],
        sphericalHarmonics[8],
    });
    Ref<Shader> prefilterCubemapShader = app->getShaderRegistry()->load("../assets/shaders/prefilter-cubemap");
    Ref<TextureCube> prefilteredSkybox = renderer->createPrefilteredCubemap(skyboxCubeToneMapped, prefilterCubemapShader, 256);
    renderer->setPrefilteredEnvMap(prefilteredSkybox);
    this->skybox = std::make_shared<SkyboxCube>(renderer, skyboxShader, skyboxCubeTexture);
    Ref<Shader> brdfLUTShader = app->getShaderRegistry()->load("../assets/shaders/brdf-lut");
    Ref<Texture2D> brdfLUT = renderer->createBRDFLUT(brdfLUTShader, 512);
    renderer->setBRDFLUT(brdfLUT);

    this->directionalLight = std::make_shared<DirectionalLight>(Rotation(-70, 90, 0), 2.86f);
    Ref<Entity> lightEntity = this->scene->createEntity();
    Ref<Entity> lightMeshEntity = this->scene->createEntity();
    lightEntity->add<Script>(std::make_shared<LightScript>(app, lightEntity, this->directionalLight, lightMeshEntity));
    lightEntity->add<Transform>();

    renderer->setCamera(this->scene->getCamera());

    this->addEntitiesForModels(renderer, "../assets/models/sand_rocks/sand_rocks_small_01_1k.gltf", {0.0f, 0.0f, 3.0f});
    this->addEntitiesForModels(renderer, "../assets/models/fountain.glb", {-4.0f, 0.2f, 4.0f});
    this->addEntitiesForModels(renderer, "../assets/models/wooden_stool/wooden_stool_02_1k.gltf", {0.0f, 0.0f, 3.0f}, Rotation(), glm::vec3(4.0f));
    this->addEntitiesForModels(renderer, "../assets/models/covered_car/covered_car_1k.gltf", {2.5f, 0.0f, 3.0f}, Rotation(0, -15, 0));
    this->addEntitiesForModels(renderer, "../assets/models/picnic_table/wooden_picnic_table_1k.gltf", {0.0f, 0.0f, 6.0f}, Rotation(4, -85, 0));

    {
        Model model = ModelImporter::importFromFile(renderer, "../assets/models/lamp.glb")[0];
        const VertexData* vertexData = model.vertices.data();
        auto vertexDataFloats = reinterpret_cast<const float*>(vertexData);
        lightMeshEntity->add<Mesh>(renderer, vertexDataFloats, model.vertices.size() * sizeof(VertexData), model.indexes.data(), model.indexes.size(), model.material,
                                   model.transformationMatrix);
        lightMeshEntity->add<Transform>(glm::vec3(0.0, 3.0f, 0.0f), Rotation(), glm::vec3(1.0f));
    }

    Model sphereModel = std::move(ModelImporter::importFromFile(renderer, "../assets/models/sphere.glb")[0]);
    for (int x = 0; x < 10; x++) {
        for (int z = 0; z < 10; z++) {
            const float xPos = -5.0f + x + 0.5f;
            const float zPos = -5.0f + z + 0.5f;
            const VertexData* vertexData = sphereModel.vertices.data();
            auto vertexDataFloats = reinterpret_cast<const float*>(vertexData);
            unsigned char roughness = static_cast<unsigned char>(static_cast<float>(9 - x) / 9 * 255.0f);
            unsigned char metallic = static_cast<unsigned char>(static_cast<float>(9 - z) / 9 * 255.0f);
            Material material(renderer->createTexture2D(4, 1, 1, 1, std::array<unsigned char, 4>{250, 0, 0, 255}.data()),
                              renderer->createTexture2D(3, 1, 1, 1, std::array<unsigned char, 3>{255, roughness, metallic}.data()));
            Ref<Entity> entity = this->scene->createEntity();
            entity->add<Mesh>(renderer, vertexDataFloats, sphereModel.vertices.size() * sizeof(VertexData), sphereModel.indexes.data(), sphereModel.indexes.size(), material,
                              sphereModel.transformationMatrix);
            entity->add<Transform>(glm::vec3(xPos, 4.0f, zPos), Rotation(), glm::vec3(0.4f));
        }
    }

    // point lights
    for (glm::vec3 positions[] = {{1.0f, 1.0f, 1.0f}, {4.0f, 4.2f, 2.0f}}; const glm::vec3& position : positions) {
        Ref<Entity> pointLightEntity = this->scene->createEntity();
        pointLightEntity->add<PointLight>(position, glm::vec3(0.2f, 0.2f, 1.0f), 10.0f);
        pointLightEntity->add<Transform>(position, Rotation(), glm::vec3(0.1f));
        const VertexData* vertexData = sphereModel.vertices.data();
        auto vertexDataFloats = reinterpret_cast<const float*>(vertexData);
        auto material = Material(renderer->createTexture2D(4, 1, 1, 1, std::array<unsigned char, 4>{50, 50, 255, 255}.data()));
        material.ignoreLighting = true;
        pointLightEntity->add<Mesh>(renderer, vertexDataFloats, sphereModel.vertices.size() * sizeof(VertexData), sphereModel.indexes.data(), sphereModel.indexes.size(), material,
                                    sphereModel.transformationMatrix);
    }
}

void SceneLayer::update(const std::unique_ptr<Context>& ctx) {
    DE_PROFILE_FUNCTION();
    ctx->renderer->beginFrame();

    // add point lights to the renderer
    const auto pointLightsView = this->scene->getPointLights();
    for (const auto& entity : pointLightsView) {
        PointLight& light = pointLightsView.get<PointLight>(entity);
        ctx->renderer->addPointLight(light);
    }

    // render meshes
    const auto meshesView = this->scene->getMeshes();
    for (const auto& entity : meshesView) {
        const Transform& transform = meshesView.get<Transform>(entity);
        const Mesh& mesh = meshesView.get<Mesh>(entity);

        const glm::mat4 translationMat = translate(glm::mat4(1.0f), transform.position);
        const glm::mat4 rotationMat = toMat4(transform.rotation.toQuaternion());
        const glm::mat4 scaleMat = scale(translationMat * rotationMat, transform.scale);
        const glm::mat4 transformMat = scaleMat * mesh.transformationMatrix;
        if (mesh.material.albedo) {
            ctx->renderer->draw(mesh.vertexArray, this->shader, transformMat, this->directionalLight, mesh.material);
        } else {
            ctx->renderer->draw(mesh.vertexArray, this->shader, transformMat, this->directionalLight);
        }
    }

    // render skybox
    ctx->renderer->drawSkybox(this->skybox);

    ctx->renderer->endFrame();

    const auto scriptsView = this->scene->getScripts();
    for (const auto& entity : scriptsView) {
        Script& script = scriptsView.get<Script>(entity);
        script.entityScript->onUpdate(ctx->deltaTime);
    }
}

void SceneLayer::addEntitiesForModels(const Ref<Renderer>& renderer, const std::string& path, const glm::vec3 position, const Rotation rotation, const glm::vec3 scale) const {
    const std::vector<Model> models = ModelImporter::importFromFile(renderer, path);
    this->addEntitiesForModels(renderer, models, position, rotation, scale);
}

void SceneLayer::addEntitiesForModels(const Ref<Renderer>& renderer, const std::vector<Model>& models, const glm::vec3 position, const Rotation rotation,
                                      const glm::vec3 scale) const {
    for (const Model& model : models) {
        const VertexData* vertexData = model.vertices.data();
        auto vertexDataFloats = reinterpret_cast<const float*>(vertexData);
        const Material& material = model.material;
        Ref<Entity> entity = this->scene->createEntity();
        entity->add<Mesh>(renderer, vertexDataFloats, model.vertices.size() * sizeof(VertexData), model.indexes.data(), model.indexes.size(), material, model.transformationMatrix);
        entity->add<Transform>(position, rotation, scale);
    }
}
