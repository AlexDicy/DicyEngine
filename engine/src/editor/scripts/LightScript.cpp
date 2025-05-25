#include "pch/enginepch.h"
#include "LightScript.h"


LightScript::LightScript(const Ref<Application>& app, const Ref<Entity>& entity, const Ref<DirectionalLight>& light, const Ref<Entity>& meshEntity) :
    EntityScript(app, entity), light(light), meshEntity(meshEntity) {}

constexpr float rotationSpeed = 15.0f;

void LightScript::onUpdate(const float deltaTime) {
    DE_PROFILE_FUNCTION();
    const float speedDelta = rotationSpeed * deltaTime;
    Rotation& rotation = this->light->getRotation();
    rotation.setYaw(rotation.getYaw() + speedDelta);

    const glm::vec3 direction = rotation.toDirection();
    this->getComponent<Transform>().setPosition(direction * glm::vec3(0.0f, 1.0f, 0.0f) + glm::vec3(0.0f, 4.0f, 4.0f));
    this->getComponent<Transform>().setRotation(Rotation(-rotation.getPitch(), rotation.getYaw(), -rotation.getRoll()));

    Rotation& meshRotation = this->meshEntity->get<Transform>().getRotation();
    meshRotation.setPitch(90 + rotation.getPitch());
    meshRotation.setYaw(rotation.getYaw());
    meshRotation.setRoll(rotation.getRoll());
}
