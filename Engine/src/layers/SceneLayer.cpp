#include "pch/enginepch.h"
#include "SceneLayer.h"

#include <filesystem>

#include "editor/scripts/CameraScript.h"
#include "editor/scripts/LightScript.h"
#include "editor/scripts/UIScript.h"
#include "images/Image.h"
#include "images/ImageUtils.h"
#include "images/LinearImage.h"
#include "rendering/skybox/SphericalHarmonics.h"
#include "scene/models/ModelImporter.h"
#include "scene/models/Plane.h"
#include "serialization/EntitySerializer.h"
#include "serialization/SceneDeserializer.h"
#include "serialization/SceneSerializer.h"

#include <toml++/impl/table.hpp>


SceneLayer::SceneLayer(const std::unique_ptr<Context>& ctx) {
    const Ref<Application>& app = ctx->app;
    const Ref<Renderer>& renderer = app->getRenderer();
    this->scene = std::make_shared<Scene>();

    // x,y,z indicator
    std::vector<Model> xyzModels = ModelImporter::importFromFile(renderer, "../assets/models/arrows.glb");
    for (Model& arrow : xyzModels) {
        arrow.material.ignoreLighting = true;
    }
    (void)this->addEntitiesForModels(renderer, xyzModels, {0.0f, 0.0f, 0.0f});

    // camera
    app->getEntityScriptRegistry()->registerScriptNative<CameraScript>("CameraScript");
    this->cameraEntity = this->scene->createEntity();
    this->cameraEntity->setTransform(glm::vec3(0.0f, 2.0f, 0.0f), Rotation(-15, 0, 0));
    this->cameraEntity->add<Script>("CameraScript", app, this->cameraEntity);

    this->shader = app->getShaderRegistry()->load("../assets/shaders/default-shader");
    renderer->setDirectionalShadowMapShader(app->getShaderRegistry()->load("../assets/shaders/shadow-map-directional"));
    renderer->setPointLightShadowMapShader(app->getShaderRegistry()->load("../assets/shaders/shadow-map-point-light"));
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

    renderer->setCamera(this->scene->getCamera());

    {
        Model model = ModelImporter::importFromFile(renderer, "../assets/models/lamp.glb")[0];
        const VertexData* vertexData = model.vertices.data();
        auto vertexDataFloats = reinterpret_cast<const float*>(vertexData);
        lightMeshEntity->add<Mesh>(renderer, vertexDataFloats, model.vertices.size() * sizeof(VertexData), model.indexes.data(), model.indexes.size(), model.material,
                                   model.transformationMatrix);
        lightMeshEntity->setTransform(glm::vec3(0.0, 3.0f, 0.0f), Rotation(), glm::vec3(1.0f));
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
            entity->setTransform(glm::vec3(xPos, 4.0f, zPos), Rotation(), glm::vec3(0.4f));
        }
    }

    // point lights
    for (glm::vec3 positions[] = {{1.0f, 1.0f, 1.0f}, {4.0f, 4.2f, 2.0f}}; const glm::vec3& position : positions) {
        Ref<Entity> pointLightEntity = this->scene->createEntity();
        pointLightEntity->add<PointLight>(position, glm::vec3(0.1f, 0.1f, 1.0f), 10.0f);
        pointLightEntity->setTransform(position, Rotation(), glm::vec3(0.1f));
        const VertexData* vertexData = sphereModel.vertices.data();
        auto vertexDataFloats = reinterpret_cast<const float*>(vertexData);
        auto material = Material(renderer->createTexture2D(4, 1, 1, 1, std::array<unsigned char, 4>{50, 50, 255, 255}.data()));
        material.ignoreLighting = true;
        pointLightEntity->add<Mesh>(renderer, vertexDataFloats, sphereModel.vertices.size() * sizeof(VertexData), sphereModel.indexes.data(), sphereModel.indexes.size(), material,
                                    sphereModel.transformationMatrix);
    }

    app->getEntityScriptRegistry()->registerScriptJava("com.dicydev.engine.scene.scripts.RotatingEntityScript");

    Ref<Entity> uiEntity = this->scene->createEntity();
    Script& uiScript = uiEntity->add<Script>(std::make_shared<UIScript>(app, uiEntity));
    const auto uiMaterial = Material(renderer->createTexture2D(4, 1, 1, 1, BGRA, std::array<unsigned char, 4>{0, 0, 0, 0}.data()));
    this->uiMesh = Plane::create(renderer, uiMaterial);
    uiEntity->add<UITexture>(uiMaterial.albedo);
    this->uiShader = app->getShaderRegistry()->load("../assets/shaders/ui");
    uiScript.getEntityScript()->onSpawn(); // todo: should not be called manually

    toml::table in = toml::parse_file("../assets/scene.toml");
    SceneDeserializer::deserialize(ctx, *this->scene, in);
}

void SceneLayer::update(const std::unique_ptr<Context>& ctx) {
    DE_PROFILE_FUNCTION();
    ctx->renderer->beginFrame();

    ctx->renderer->setDirectionalLight(this->directionalLight);

    // add point lights to the renderer
    const auto pointLightsView = this->scene->getPointLights();
    for (const auto& entity : pointLightsView) {
        PointLight& light = pointLightsView.get<PointLight>(entity);
        ctx->renderer->addPointLight(light);
    }

    // render meshes for shadow mapping
    ctx->renderer->beginDirectionalShadows();
    const auto meshesView = this->scene->getMeshes();
    for (const auto& entity : meshesView) {
        Transform& transform = meshesView.get<Transform>(entity);
        const Mesh& mesh = meshesView.get<Mesh>(entity);

        ctx->renderer->drawForDirectionalShadows(mesh.vertexArray, transform.getAsMatrix() * mesh.transformationMatrix);
    }

    // render meshes for point lights shadow mapping
    ctx->renderer->beginPointLightShadows();
    for (int i = 0; i < pointLightsView.size(); i++) {
        PointLight& light = pointLightsView.get<PointLight>(pointLightsView.begin()[i]);
        for (int face = 0; face < 6; face++) {
            ctx->renderer->beginPointLightShadow(light, i, face);
            for (const auto& entity : meshesView) {
                Transform& transform = meshesView.get<Transform>(entity);
                const Mesh& mesh = meshesView.get<Mesh>(entity);

                ctx->renderer->drawForPointLightShadows(mesh.vertexArray, transform.getAsMatrix() * mesh.transformationMatrix);
            }
        }
    }
    ctx->renderer->endShadows();

    // render meshes
    for (const auto& entity : meshesView) {
        Transform& transform = meshesView.get<Transform>(entity);
        const Mesh& mesh = meshesView.get<Mesh>(entity);

        const glm::mat4 transformMat = transform.getAsMatrix() * mesh.transformationMatrix;
        if (mesh.material.albedo) {
            ctx->renderer->draw(mesh.vertexArray, transformMat, this->shader, mesh.material);
        } else {
            ctx->renderer->draw(mesh.vertexArray, transformMat, this->shader);
        }
    }

    ctx->renderer->drawSkybox(this->skybox);
    ctx->renderer->drawUI(this->uiMesh->vertexArray, this->uiShader, this->uiMesh->material);

    ctx->renderer->endFrame();

    const auto scriptsView = this->scene->getScripts();
    for (const auto& entity : scriptsView) {
        Script& script = scriptsView.get<Script>(entity);
        script.getEntityScript()->onUpdate(ctx->deltaTime);
    }
}

std::vector<Ref<Entity>> SceneLayer::addEntitiesForModels(const Ref<Renderer>& renderer, const std::string& path, const glm::vec3 position, const Rotation rotation,
                                                          const glm::vec3 scale) const {
    const std::vector<Model> models = ModelImporter::importFromFile(renderer, path);
    return this->addEntitiesForModels(renderer, models, position, rotation, scale);
}

std::vector<Ref<Entity>> SceneLayer::addEntitiesForModels(const Ref<Renderer>& renderer, const std::vector<Model>& models, const glm::vec3 position, const Rotation rotation,
                                                          const glm::vec3 scale) const {
    std::vector<Ref<Entity>> entities;
    for (const Model& model : models) {
        const VertexData* vertexData = model.vertices.data();
        auto vertexDataFloats = reinterpret_cast<const float*>(vertexData);
        const Material& material = model.material;
        Ref<Entity> entity = this->scene->createEntity();
        this->scene->setEntityModel(entity, model);
        entity->add<Mesh>(renderer, vertexDataFloats, model.vertices.size() * sizeof(VertexData), model.indexes.data(), model.indexes.size(), material, model.transformationMatrix);
        entity->setTransform(position, rotation, scale);
        entities.push_back(entity);
    }
    return entities;
}
