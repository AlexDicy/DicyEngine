#include "pch/enginepch.h"
#include "camera_script.h"

#include "window.h"
#include "input/input.h"
#include "rendering/camera/orthographic_camera.h"
#include "rendering/camera/perspective_camera.h"

Ref<PerspectiveCamera> get_perspective_camera(const Ref<Window>&);
Ref<OrthographicCamera> get_orthographic_camera();

CameraScript::CameraScript(const Ref<Application>& app, const Ref<Entity>& entity) : EntityScript(app, entity) {
    this->script = EntityScriptJava::create(app, entity, "com/dicydev/engine/scene/scripts/CameraScript");
    this->transform = &get_component<Transform>();

    const Ref<Window>& window = app->get_window();
    const Ref<Renderer>& renderer = app->get_renderer();
    this->camera = get_perspective_camera(window);

    Input::set_action("change_camera", InputCode::KEY_O);
    Input::set_action("move_camera_up", InputCode::KEY_E);
    Input::set_action("move_camera_down", InputCode::KEY_Q);
    Input::set_action("move_camera_faster", InputCode::KEY_LEFT_SHIFT);

    constexpr float sensitivity = 0.16f;
    Input::bind_axis("look_x", [this](const float delta_x) {
        if (Input::is_action_pressed("right_click")) {
            this->transform->rotation.yaw = this->transform->rotation.yaw + delta_x * sensitivity;
        }
    });
    Input::bind_axis("look_y", [this](const float delta_y) {
        if (Input::is_action_pressed("right_click")) {
            const float pitch = glm::clamp(this->transform->rotation.pitch + delta_y * sensitivity, -90.0f, 90.0f);
            this->transform->rotation.pitch = pitch;
        }
    });
    Input::bind_action_pressed("change_camera", [this, renderer, window] {
        static bool camera_bool = false;
        if (camera_bool) {
            this->camera = get_perspective_camera(window);
        } else {
            this->camera = get_orthographic_camera();
        }
        renderer->set_camera(this->camera);
        camera_bool = !camera_bool;
    });

    Input::bind_action_pressed("move_camera_faster", [this] {
        this->move_faster = true;
    });
    Input::bind_action_released("move_camera_faster", [this] {
        this->move_faster = false;
    });
}

constexpr float damping_factor = 10.0f;
constexpr float base_acceleration = 50.0f;

void CameraScript::on_update(const float delta_time) {
    DE_PROFILE_FUNCTION();
    this->script->on_update(delta_time);
    // slow down
    this->velocity *= std::max(0.0f, 1.0f - damping_factor * delta_time);
    // direction
    const float yaw = glm::radians(this->transform->rotation.yaw);
    const auto forward = glm::vec3(sinf(yaw), 0.0f, cosf(yaw));
    const auto right = glm::vec3(forward.z, 0.0f, -forward.x);

    glm::vec3 movement_direction(0.0f);

    if (Input::is_action_pressed("move_left")) {
        movement_direction -= right;
    }
    if (Input::is_action_pressed("move_right")) {
        movement_direction += right;
    }
    if (Input::is_action_pressed("move_forward")) {
        movement_direction += forward;
    }
    if (Input::is_action_pressed("move_backward")) {
        movement_direction -= forward;
    }
    if (Input::is_action_pressed("move_camera_up")) {
        movement_direction.y += 1.0f;
    }
    if (Input::is_action_pressed("move_camera_down")) {
        movement_direction.y -= 1.0f;
    }

    if (length(movement_direction) > 0.0f) {
        const float acceleration = this->move_faster ? base_acceleration * 2.0f : base_acceleration;
        this->velocity += normalize(movement_direction) * acceleration * delta_time;
    }

    this->transform->position += this->velocity * delta_time;

    this->camera->set_position(this->transform->position);
    this->camera->set_rotation(this->transform->rotation);
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
