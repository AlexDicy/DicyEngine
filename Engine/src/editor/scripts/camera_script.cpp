#include "pch/enginepch.h"
#include "camera_script.h"

#include "window.h"
#include "input/input.h"
#include "rendering/camera/orthographic_camera.h"
#include "rendering/camera/perspective_camera.h"
#include "scene/scene.h"

Ref<PerspectiveCamera> get_perspective_camera(const Ref<Window>&);
Ref<OrthographicCamera> get_orthographic_camera();

CameraScript::CameraScript(const Ref<Application>& app, const Ref<Entity>& entity) : EntityScript(app, entity) {
    this->script = EntityScriptJava::create(app, entity, "com/dicydev/engine/scene/scripts/CameraScript");

    const Ref<Window>& window = app->get_window();
    const Ref<Renderer>& renderer = app->get_renderer();
    const Ref<Scene>& scene = entity->get_scene();
    scene->set_camera(get_perspective_camera(window));

    Input::set_action("change_camera", InputCode::KEY_O);

    Input::bind_action_pressed("change_camera", [this, renderer, window, scene] {
        static bool camera_bool = false;
        if (camera_bool) {
            scene->set_camera(get_perspective_camera(window));
        } else {
            scene->set_camera(get_orthographic_camera());
        }
        renderer->set_camera(scene->get_camera());
        camera_bool = !camera_bool;
    });
}

void CameraScript::on_update(const float delta_time) {
    DE_PROFILE_FUNCTION();
    this->script->on_update(delta_time);
}

void CameraScript::on_spawn() {
    this->script->on_spawn();
}

void CameraScript::on_destroy() {
    this->script->on_destroy();
}

void CameraScript::on_awake() {
    this->script->on_awake();
}

void CameraScript::on_sleep() {
    this->script->on_sleep();
}

Ref<PerspectiveCamera> get_perspective_camera(const Ref<Window>& window) {
    return std::make_shared<PerspectiveCamera>(90.0f, static_cast<float>(window->get_width()) / static_cast<float>(window->get_height()));
}

Ref<OrthographicCamera> get_orthographic_camera() {
    return std::make_shared<OrthographicCamera>(-1.6f, 1.6f, -0.9f, 0.9f);
}
