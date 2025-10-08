#include "pch/enginepch.h"
#include "SceneLayer.h"

#include <filesystem>

#include "editor/scripts/CameraScript.h"
#include "editor/scripts/LightScript.h"
#include "images/Image.h"
#include "images/ImageUtils.h"
#include "images/LinearImage.h"
#include "rendering/skybox/SphericalHarmonics.h"
#include "scene/models/ModelImporter.h"
#include "serialization/SceneDeserializer.h"

#include <numbers>
#include <toml++/impl/table.hpp>
#include <utility>


SceneLayer::SceneLayer(const std::unique_ptr<Context>& ctx) : Layer(ctx) {
    const Ref<Application>& app = ctx->app;
    const Ref<Renderer>& renderer = app->getRenderer();

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
    // this takes more than 2 seconds, might be worth to improve
    Ref<LinearImage> skyboxToneMapped = ImageUtils::acesFilmicTonemapping(skyboxHDR);
    Ref<Texture> skyboxTexture = Texture::builder().fromImage(skyboxHDR).build(renderer);
    Ref<Texture> skyboxTextureToneMapped = Texture::builder().fromImage(skyboxToneMapped).build(renderer);
    Ref<Shader> equirectangularToCubemapShader = app->getShaderRegistry()->load("../assets/shaders/equirectangular-to-cubemap");
    Ref<Texture> skyboxCubeTexture = renderer->createTextureCubeFromHDR(skyboxTexture, equirectangularToCubemapShader, 2048);
    Ref<Texture> skyboxCubeToneMapped = renderer->createTextureCubeFromHDR(skyboxTextureToneMapped, equirectangularToCubemapShader, 256);
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
    Ref<Texture> prefilteredSkybox = renderer->createPrefilteredCubemap(skyboxCubeToneMapped, prefilterCubemapShader, 256);
    renderer->setPrefilteredEnvMap(prefilteredSkybox);
    this->skybox = std::make_shared<SkyboxCube>(renderer, skyboxShader, skyboxCubeTexture);
    Ref<Shader> brdfLUTShader = app->getShaderRegistry()->load("../assets/shaders/brdf-lut");
    Ref<Texture> brdfLUT = renderer->createBRDFLUT(brdfLUTShader, 512);
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

    Ref<Entity> spheres = this->scene->createEntity("Red spheres");
    Model sphereModel = std::move(ModelImporter::importFromFile(renderer, "../assets/models/sphere.glb")[0]);
    Ref<Texture> redTexture = Texture::builder()
                                  .width(1)
                                  .height(1)
                                  .format(Texture::Format::RGB)
                                  .internalFormat(Texture::InternalFormat::RGB8)
                                  .data(std::array<unsigned char, 3>{250, 0, 0}.data())
                                  .build(renderer);
    for (int x = 0; x < 10; x++) {
        for (int z = 0; z < 10; z++) {
            const float xPos = -5.0f + x + 0.5f;
            const float zPos = -5.0f + z + 0.5f;
            const VertexData* vertexData = sphereModel.vertices.data();
            auto vertexDataFloats = reinterpret_cast<const float*>(vertexData);
            unsigned char roughness = static_cast<unsigned char>(static_cast<float>(9 - x) / 9 * 255.0f);
            unsigned char metallic = static_cast<unsigned char>(static_cast<float>(9 - z) / 9 * 255.0f);
            Material material(redTexture,
                              Texture::builder()
                                  .width(1)
                                  .height(1)
                                  .format(Texture::Format::RGB)
                                  .internalFormat(Texture::InternalFormat::RGB8)
                                  .data(std::array<unsigned char, 3>{255, roughness, metallic}.data())
                                  .build(renderer));
            Ref<Entity> entity = this->scene->createEntity(std::format("Sphere ({},{})", x, z));
            entity->setParent(spheres);
            entity->add<Mesh>(renderer, vertexDataFloats, sphereModel.vertices.size() * sizeof(VertexData), sphereModel.indexes.data(), sphereModel.indexes.size(), material,
                              sphereModel.transformationMatrix);
            entity->setTransform(glm::vec3(xPos, 4.0f, zPos), Rotation(), glm::vec3(0.4f));
            entity->add<RigidBody>(new SphereShape(0.5f), Physics::Layer::MOVING);
        }
    }

    // invisible floor
    Ref<Entity> floor = this->scene->createEntity("Floor");
    floor->setTransform(glm::vec3(0.0f), Rotation(), glm::vec3(1.0f));
    floor->add<RigidBody>(new BoxShape(glm::vec3(5.0f, 0.1f, 5.0f)), Physics::Layer::NON_MOVING);

    // point lights
    for (glm::vec3 positions[] = {{1.0f, 1.0f, 1.0f}, {4.0f, 4.2f, 2.0f}}; const glm::vec3& position : positions) {
        Ref<Entity> pointLightEntity = this->scene->createEntity("Point Light");
        pointLightEntity->add<PointLight>(position, glm::vec3(0.1f, 0.1f, 1.0f), 10.0f);
        pointLightEntity->setTransform(position, Rotation(), glm::vec3(0.1f));
        const VertexData* vertexData = sphereModel.vertices.data();
        auto vertexDataFloats = reinterpret_cast<const float*>(vertexData);
        auto material = Material(Texture::builder()
                                     .width(1)
                                     .height(1)
                                     .format(Texture::Format::RGB)
                                     .internalFormat(Texture::InternalFormat::RGB8)
                                     .data(std::array<unsigned char, 3>{50, 50, 255}.data())
                                     .build(renderer));
        material.ignoreLighting = true;
        pointLightEntity->add<Mesh>(renderer, vertexDataFloats, sphereModel.vertices.size() * sizeof(VertexData), sphereModel.indexes.data(), sphereModel.indexes.size(), material,
                                    sphereModel.transformationMatrix);
    }

    app->getEntityScriptRegistry()->registerScriptJava("com.dicydev.engine.scene.scripts.RotatingEntityScript");

    toml::table in = toml::parse_file("../assets/scene.toml");
    SceneDeserializer::deserialize(ctx, *this->scene, in);

    Input::setAction("play", InputCode::KEY_P);
    Input::bindActionPressed("play", [this, &ctx] {
        this->play(ctx);
    });
}

void SceneLayer::play(const std::unique_ptr<Context>& ctx) {
    const auto rigidBodiesView = this->scene->getRigidBodies();
    for (const auto& entity : rigidBodiesView) {
        RigidBody& rigidBody = rigidBodiesView.get<RigidBody>(entity);
        if (rigidBody.isInitialized()) {
            continue;
        }
        Transform& transform = rigidBodiesView.get<Transform>(entity);
        rigidBody.initializeBody(ctx->physics, transform);
        ctx->physics->addBody(rigidBody.getPhysicsBody());
    }
}

void SceneLayer::update(const std::unique_ptr<Context>& ctx) {
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

        const unsigned int entityId = entt::to_integral(entity);

        const glm::mat4 transformMat = transform.getAsMatrix() * mesh.transformationMatrix;
        if (mesh.material.albedo) {
            ctx->renderer->draw(entityId, mesh.vertexArray, transformMat, this->shader, mesh.material);
        } else {
            ctx->renderer->draw(entityId, mesh.vertexArray, transformMat, this->shader);
        }
    }
    ctx->renderer->endMeshes();

    ctx->renderer->drawSkybox(this->skybox);
}
