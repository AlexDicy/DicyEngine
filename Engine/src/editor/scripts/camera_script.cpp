#include "camera_script.h"

#include "window.h"
#include "input/input.h"
#include "rendering/camera/orthographic_camera.h"
#include "rendering/camera/perspective_camera.h"

Ref<PerspectiveCamera> get_perspective_camera(const Ref<Window>&);
Ref<OrthographicCamera> get_orthographic_camera();

CameraScript::CameraScript(const Application* app, const Ref<Entity>& entity) : EntityScript(app, entity) {
    this->transform = &get_component<Transform>();

    const Ref<Window>& window = app->get_window();
    const Ref<Renderer>& renderer = app->get_renderer();
    this->camera = get_perspective_camera(window);

    Input::set_action("change_camera", InputCode::KEY_O);
    Input::set_action("move_camera_up", InputCode::KEY_E);
    Input::set_action("move_camera_down", InputCode::KEY_Q);

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
}

constexpr float camera_speed = 2.0f;

void CameraScript::on_update(const float delta_time) {
    DE_PROFILE_FUNCTION();
    const float speed_delta = camera_speed * delta_time;
    glm::vec3& position = this->transform->position;
    const float yaw = glm::radians(this->transform->rotation.yaw);
    const auto forward = glm::vec3(sinf(yaw), 0, cosf(yaw));
    const auto right = glm::vec3(forward.z, 0, -forward.x);

    if (Input::is_action_pressed("move_left")) {
        position -= right * speed_delta;
    }
    if (Input::is_action_pressed("move_right")) {
        position += right * speed_delta;
    }
    if (Input::is_action_pressed("move_forward")) {
        position += forward * speed_delta;
    }
    if (Input::is_action_pressed("move_backward")) {
        position -= forward * speed_delta;
    }
    if (Input::is_action_pressed("move_camera_up")) {
        position.y += speed_delta;
    }
    if (Input::is_action_pressed("move_camera_down")) {
        position.y -= speed_delta;
    }

    this->camera->set_position(this->transform->position);
    this->camera->set_rotation(this->transform->rotation);
}

Ref<PerspectiveCamera> get_perspective_camera(const Ref<Window>& window) {
    return std::make_shared<PerspectiveCamera>(90.0f, static_cast<float>(window->get_width()) / static_cast<float>(window->get_height()));
}

Ref<OrthographicCamera> get_orthographic_camera() {
    return std::make_shared<OrthographicCamera>(-1.6f, 1.6f, -0.9f, 0.9f);
}