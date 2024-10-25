#include "pch/enginepch.h"
#include "LightScript.h"


LightScript::LightScript(const Ref<Application>& app, const Ref<Entity>& entity, const Ref<DirectionalLight>& light, const Ref<Entity>& meshEntity) :
    EntityScript(app, entity), light(light), meshEntity(meshEntity) {}

constexpr float rotationSpeed = 15.0f;

void LightScript::onUpdate(const float deltaTime) {
    DE_PROFILE_FUNCTION();
    const float speedDelta = rotationSpeed * deltaTime;
    this->light->getRotation().yaw += speedDelta;

    const Rotation rotation = this->light->getRotation();
    const glm::vec3 direction = rotation.toDirection();
    this->getComponent<Transform>().position = direction * glm::vec3(0.0f, 1.0f, 0.0f) + glm::vec3(0.0f, 4.0f, 4.0f);
    this->getComponent<Transform>().rotation = Rotation(-rotation.pitch, rotation.yaw, -rotation.roll);

    this->meshEntity->get<Transform>().rotation = Rotation(90 + rotation.pitch, rotation.yaw, rotation.roll);
}
