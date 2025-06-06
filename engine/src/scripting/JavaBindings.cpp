﻿#include "pch/enginepch.h"
#include "JavaBindings.h"

#include "JavaBridge.h"
#include "scene/Scene.h"

#include <entt/entity/registry.hpp>

void inputSetAction(JNIEnv* env, jobject, const jstring action, jint key) {
    const char* actionStr = env->GetStringUTFChars(action, nullptr);
    Input::setAction(actionStr, static_cast<InputCode>(key));
    env->ReleaseStringUTFChars(action, actionStr);
}

jboolean inputIsActionPressed(JNIEnv* env, jobject, const jstring action) {
    const char* actionStr = env->GetStringUTFChars(action, nullptr);
    const jboolean result = Input::isActionPressed(actionStr);
    env->ReleaseStringUTFChars(action, actionStr);
    return result;
}

void inputBindAxis(JNIEnv* env, jobject, const jstring axis, const jobject callback) {
    const char* axisStr = env->GetStringUTFChars(axis, nullptr);
    const auto callbackRef = env->NewGlobalRef(callback);

    Input::bindAxis(axisStr, [env, callbackRef](const float delta) {
        env->CallVoidMethod(callbackRef, env->GetMethodID(env->GetObjectClass(callbackRef), "callback", "(F)V"), delta);
    });

    env->ReleaseStringUTFChars(axis, axisStr);
}

void inputBindActionPressed(JNIEnv* env, jobject, const jstring action, const jobject callback) {
    const char* actionStr = env->GetStringUTFChars(action, nullptr);
    const auto callbackRef = env->NewGlobalRef(callback);

    Input::bindActionPressed(actionStr, [env, callbackRef] {
        env->CallVoidMethod(callbackRef, env->GetMethodID(env->GetObjectClass(callbackRef), "callback", "()V"));
    });

    env->ReleaseStringUTFChars(action, actionStr);
}

void inputBindActionReleased(JNIEnv* env, jobject, const jstring action, const jobject callback) {
    const char* actionStr = env->GetStringUTFChars(action, nullptr);
    const auto callbackRef = env->NewGlobalRef(callback);

    Input::bindActionReleased(actionStr, [env, callbackRef] {
        env->CallVoidMethod(callbackRef, env->GetMethodID(env->GetObjectClass(callbackRef), "callback", "()V"));
    });

    env->ReleaseStringUTFChars(action, actionStr);
}


jobject nativeRegistryGetComponentBuffer(JNIEnv* env, jobject, const jlong registry, jint entityId, jint componentId) {
    const auto* registryPointer = reinterpret_cast<entt::registry*>(registry); // NOLINT(performance-no-int-to-ptr)
    const auto entity = static_cast<entt::entity>(entityId);

    // just hard code component type to be Transform for now
    const Transform& component = registryPointer->get<Transform>(entity);
    constexpr auto componentSize = sizeof(Transform);
    return env->NewDirectByteBuffer(const_cast<Transform*>(&component), componentSize);
}

void JavaBindings::transformInvalidateGlobal(JNIEnv* env, jobject, const jobject directBuffer) {
    auto* transform = static_cast<Transform*>(env->GetDirectBufferAddress(directBuffer));
    transform->invalidateGlobal();
}

jobject sceneGetCamera(JNIEnv* env, const jobject sceneObject) {
    const jclass sceneClass = env->GetObjectClass(sceneObject);
    const jlong scenePointer = env->GetLongField(sceneObject, env->GetFieldID(sceneClass, "pointer", "J"));
    const auto* scene = reinterpret_cast<Scene*>(scenePointer); // NOLINT(performance-no-int-to-ptr)
    const jobject cameraObject = JavaClass("com/dicydev/engine/scene/camera/Camera").newInstance("(J)V", scene->getCamera().get());
    return cameraObject;
}

jobject cameraGetPosition(JNIEnv* env, jobject, const jlong camera) {
    const auto* cameraPointer = reinterpret_cast<Camera*>(camera); // NOLINT(performance-no-int-to-ptr)
    const glm::vec3& position = cameraPointer->getPosition();
    constexpr auto positionSize = sizeof(glm::vec3);
    return env->NewDirectByteBuffer(const_cast<glm::vec3*>(&position), positionSize);
}

jobject cameraGetRotation(JNIEnv* env, jobject, const jlong camera) {
    const auto* cameraPointer = reinterpret_cast<Camera*>(camera); // NOLINT(performance-no-int-to-ptr)
    const Rotation& rotation = cameraPointer->getRotation();
    constexpr auto rotationSize = sizeof(Rotation);
    return env->NewDirectByteBuffer(const_cast<Rotation*>(&rotation), rotationSize);
}

void JavaBindings::init() {
    const Ref<JavaBridge> bridge = JavaBridge::getInstance();
    JNIEnv* env = JavaBridge::getEnv();

    const std::array<JNINativeMethod, 5> inputMethods = {{
        {const_cast<char*>("setAction"), const_cast<char*>("(Ljava/lang/String;I)V"), reinterpret_cast<void*>(inputSetAction)},
        {const_cast<char*>("isActionPressed"), const_cast<char*>("(Ljava/lang/String;)Z"), reinterpret_cast<void*>(inputIsActionPressed)},
        {const_cast<char*>("bindAxis"), const_cast<char*>("(Ljava/lang/String;Lcom/dicydev/engine/input/InputAxisCallback;)V"), reinterpret_cast<void*>(inputBindAxis)},
        {const_cast<char*>("bindActionPressed"), const_cast<char*>("(Ljava/lang/String;Lcom/dicydev/engine/input/InputActionCallback;)V"), reinterpret_cast<void*>(inputBindActionPressed)},
        {const_cast<char*>("bindActionReleased"), const_cast<char*>("(Ljava/lang/String;Lcom/dicydev/engine/input/InputActionCallback;)V"), reinterpret_cast<void*>(inputBindActionReleased)},
    }};
    env->RegisterNatives(bridge->getClass("com/dicydev/engine/input/Input"), inputMethods.data(), inputMethods.size());

    const std::array<JNINativeMethod, 1> nativeRegistryMethods = {{
        {const_cast<char*>("getComponentBuffer"), const_cast<char*>("(JII)Ljava/nio/ByteBuffer;"), reinterpret_cast<void*>(nativeRegistryGetComponentBuffer)},
    }};
    env->RegisterNatives(bridge->getClass("com/dicydev/engine/components/NativeRegistry"), nativeRegistryMethods.data(), nativeRegistryMethods.size());

    const std::array<JNINativeMethod, 1> transformMethods = {{
        {const_cast<char*>("invalidateGlobal"), const_cast<char*>("(Ljava/nio/ByteBuffer;)V"), reinterpret_cast<void*>(transformInvalidateGlobal)},
    }};
    env->RegisterNatives(bridge->getClass("com/dicydev/engine/components/Transform"), transformMethods.data(), transformMethods.size());

    const std::array<JNINativeMethod, 1> sceneMethods = {{
        {const_cast<char*>("getCamera"), const_cast<char*>("()Lcom/dicydev/engine/scene/camera/Camera;"), reinterpret_cast<void*>(sceneGetCamera)},
    }};
    env->RegisterNatives(bridge->getClass("com/dicydev/engine/scene/Scene"), sceneMethods.data(), sceneMethods.size());

    const std::array<JNINativeMethod, 2> cameraMethods = {{
        {const_cast<char*>("getPosition"), const_cast<char*>("(J)Ljava/nio/ByteBuffer;"), reinterpret_cast<void*>(cameraGetPosition)},
        {const_cast<char*>("getRotation"), const_cast<char*>("(J)Ljava/nio/ByteBuffer;"), reinterpret_cast<void*>(cameraGetRotation)},
    }};
    env->RegisterNatives(bridge->getClass("com/dicydev/engine/scene/camera/Camera"), cameraMethods.data(), cameraMethods.size());
}
