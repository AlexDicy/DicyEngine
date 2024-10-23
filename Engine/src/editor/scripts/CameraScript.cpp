#include "pch/enginepch.h"
#include "CameraScript.h"

#include "Window.h"
#include "input/Input.h"
#include "rendering/camera/OrthographicCamera.h"
#include "rendering/camera/PerspectiveCamera.h"
#include "scene/Scene.h"

Ref<PerspectiveCamera> getPerspectiveCamera(const Ref<Window>&);
Ref<OrthographicCamera> getOrthographicCamera();

CameraScript::CameraScript(const Ref<Application>& app, const Ref<Entity>& entity) : EntityScript(app, entity) {
    this->script = EntityScriptJava::create(app, entity, "com/dicydev/engine/scene/scripts/CameraScript");

    const Ref<Window>& window = app->getWindow();
    const Ref<Renderer>& renderer = app->getRenderer();
    const Ref<Scene>& scene = entity->getScene();
    scene->setCamera(getPerspectiveCamera(window));

    Input::setAction("change_camera", InputCode::KEY_O);

    Input::bindActionPressed("change_camera", [this, renderer, window, scene] {
        static bool cameraBool = false;
        if (cameraBool) {
            scene->setCamera(getPerspectiveCamera(window));
        } else {
            scene->setCamera(getOrthographicCamera());
        }
        renderer->setCamera(scene->getCamera());
        cameraBool = !cameraBool;
    });
}

void CameraScript::onUpdate(const float deltaTime) {
    DE_PROFILE_FUNCTION();
    this->script->onUpdate(deltaTime);
}

void CameraScript::onSpawn() {
    this->script->onSpawn();
}

void CameraScript::onDestroy() {
    this->script->onDestroy();
}

void CameraScript::onAwake() {
    this->script->onAwake();
}

void CameraScript::onSleep() {
    this->script->onSleep();
}

Ref<PerspectiveCamera> getPerspectiveCamera(const Ref<Window>& window) {
    return std::make_shared<PerspectiveCamera>(58.0f, static_cast<float>(window->getWidth()) / static_cast<float>(window->getHeight()));
}

Ref<OrthographicCamera> getOrthographicCamera() {
    return std::make_shared<OrthographicCamera>(-1.6f, 1.6f, -0.9f, 0.9f);
}
