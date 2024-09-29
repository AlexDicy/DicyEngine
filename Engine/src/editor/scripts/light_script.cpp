#include "light_script.h"


LightScript::LightScript(const Application* app, const Ref<Entity>& entity, const Ref<DirectionalLight>& light) : EntityScript(app, entity), light(light) {}

constexpr float rotation_speed = 15.0f;

void LightScript::on_update(const float delta_time) {
    DE_PROFILE_FUNCTION();
    const float speed_delta = rotation_speed * delta_time;
    this->light->get_rotation().yaw += speed_delta;
}
