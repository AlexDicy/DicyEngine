#include "light_script.h"


LightScript::LightScript(const Application* app, const Ref<Entity>& entity, const Ref<DirectionalLight>& light, const Ref<Entity>& mesh_entity) :
    EntityScript(app, entity), light(light), mesh_entity(mesh_entity) {}

constexpr float rotation_speed = 15.0f;

void LightScript::on_update(const float delta_time) {
    DE_PROFILE_FUNCTION();
    const float speed_delta = rotation_speed * delta_time;
    this->light->get_rotation().yaw += speed_delta;

    const Rotation rotation = this->light->get_rotation();
    const glm::vec3 direction = rotation.to_direction();
    this->get_component<Transform>().position = direction * glm::vec3(0.0f, 1.0f, 0.0f) + glm::vec3(0.0f, 4.0f, 4.0f);
    this->get_component<Transform>().rotation = Rotation(-rotation.pitch, rotation.yaw, -rotation.roll);

    this->mesh_entity->get<Transform>().rotation = Rotation(90 + rotation.pitch, rotation.yaw, rotation.roll);
}
