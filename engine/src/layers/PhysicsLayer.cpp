#include "pch/enginepch.h"
#include "PhysicsLayer.h"

void PhysicsLayer::update(const std::unique_ptr<Context>& ctx) {
    DE_PROFILE_FUNCTION();
    // keep the physics simulation stable for FPS higher than 10
    const int steps = glm::clamp(static_cast<int>(glm::ceil(ctx->deltaTime * 60.0f)), 1, 6);
    ctx->physics->update(ctx->deltaTime, steps);
    const auto rigidBodiesView = this->scene->getRigidBodies();
    for (const auto& entity : rigidBodiesView) {
        RigidBody& rigidBody = rigidBodiesView.get<RigidBody>(entity);
        if (!rigidBody.isInitialized()) {
            continue;
        }
        Transform& transform = rigidBodiesView.get<Transform>(entity);
        ctx->physics->syncTransform(rigidBody.getPhysicsBody(), transform);
    }
}
