#include "pch/enginepch.h"
#include "java_bindings.h"

#include "java_bridge.h"
#include "scene/scene.h"

#include <entt/entity/registry.hpp>

void inputSetAction(JNIEnv* env, jobject, const jstring action, jint key) {
    const char* actionStr = env->GetStringUTFChars(action, nullptr);
    Input::set_action(actionStr, static_cast<InputCode>(key));
    env->ReleaseStringUTFChars(action, actionStr);
}

jboolean inputIsActionPressed(JNIEnv* env, jobject, const jstring action) {
    const char* actionStr = env->GetStringUTFChars(action, nullptr);
    const jboolean result = Input::is_action_pressed(actionStr);
    env->ReleaseStringUTFChars(action, actionStr);
    return result;
}

void inputBindAxis(JNIEnv* env, jobject, const jstring axis, const jobject callback) {
    const char* axisStr = env->GetStringUTFChars(axis, nullptr);
    const auto callbackRef = env->NewGlobalRef(callback);

    Input::bind_axis(axisStr, [env, callbackRef](const float delta) {
        env->CallVoidMethod(callbackRef, env->GetMethodID(env->GetObjectClass(callbackRef), "callback", "(F)V"), delta);
    });

    env->ReleaseStringUTFChars(axis, axisStr);
}

void inputBindActionPressed(JNIEnv* env, jobject, const jstring action, const jobject callback) {
    const char* actionStr = env->GetStringUTFChars(action, nullptr);
    const auto callbackRef = env->NewGlobalRef(callback);

    Input::bind_action_pressed(actionStr, [env, callbackRef] {
        env->CallVoidMethod(callbackRef, env->GetMethodID(env->GetObjectClass(callbackRef), "callback", "()V"));
    });

    env->ReleaseStringUTFChars(action, actionStr);
}

void inputBindActionReleased(JNIEnv* env, jobject, const jstring action, const jobject callback) {
    const char* actionStr = env->GetStringUTFChars(action, nullptr);
    const auto callbackRef = env->NewGlobalRef(callback);

    Input::bind_action_released(actionStr, [env, callbackRef] {
        env->CallVoidMethod(callbackRef, env->GetMethodID(env->GetObjectClass(callbackRef), "callback", "()V"));
    });

    env->ReleaseStringUTFChars(action, actionStr);
}


jobject nativeRegistryGetComponentBuffer(JNIEnv* env, jobject, const jlong registry, jint entity_id, jint component_id) {
    const auto* registryPointer = reinterpret_cast<entt::registry*>(registry); // NOLINT(performance-no-int-to-ptr)
    const auto entity = static_cast<entt::entity>(entity_id);

    // just hard code component type to be Transform for now
    const Transform& component = registryPointer->get<Transform>(entity);
    constexpr auto componentSize = sizeof(Transform);
    return env->NewDirectByteBuffer(const_cast<Transform*>(&component), componentSize);
}

jobject sceneGetCamera(JNIEnv* env, const jobject scene_object) {
    const jclass sceneClass = env->GetObjectClass(scene_object);
    const jlong scenePointer = env->GetLongField(scene_object, env->GetFieldID(sceneClass, "pointer", "J"));
    const auto* scene = reinterpret_cast<Scene*>(scenePointer); // NOLINT(performance-no-int-to-ptr)
    const jobject cameraObject = JavaClass("com/dicydev/engine/scene/camera/Camera").newInstance("(J)V", scene->get_camera().get());
    return cameraObject;
}

jobject cameraGetPosition(JNIEnv* env, jobject, const jlong camera) {
    const auto* cameraPointer = reinterpret_cast<Camera*>(camera); // NOLINT(performance-no-int-to-ptr)
    const glm::vec3& position = cameraPointer->get_position();
    constexpr auto positionSize = sizeof(glm::vec3);
    return env->NewDirectByteBuffer(const_cast<glm::vec3*>(&position), positionSize);
}

jobject cameraGetRotation(JNIEnv* env, jobject, const jlong camera) {
    const auto* cameraPointer = reinterpret_cast<Camera*>(camera); // NOLINT(performance-no-int-to-ptr)
    const Rotation& rotation = cameraPointer->get_rotation();
    constexpr auto rotationSize = sizeof(Rotation);
    return env->NewDirectByteBuffer(const_cast<Rotation*>(&rotation), rotationSize);
}

void JavaBindings::init() {
    JNIEnv* env = JavaBridge::getEnv();

    const std::array<JNINativeMethod, 5> inputMethods = {{
        {const_cast<char*>("setAction"), const_cast<char*>("(Ljava/lang/String;I)V"), reinterpret_cast<void*>(inputSetAction)},
        {const_cast<char*>("isActionPressed"), const_cast<char*>("(Ljava/lang/String;)Z"), reinterpret_cast<void*>(inputIsActionPressed)},
        {const_cast<char*>("bindAxis"), const_cast<char*>("(Ljava/lang/String;Lcom/dicydev/engine/input/InputAxisCallback;)V"), reinterpret_cast<void*>(inputBindAxis)},
        {const_cast<char*>("bindActionPressed"), const_cast<char*>("(Ljava/lang/String;Lcom/dicydev/engine/input/InputActionCallback;)V"), reinterpret_cast<void*>(inputBindActionPressed)},
        {const_cast<char*>("bindActionReleased"), const_cast<char*>("(Ljava/lang/String;Lcom/dicydev/engine/input/InputActionCallback;)V"), reinterpret_cast<void*>(inputBindActionReleased)},
    }};
    env->RegisterNatives(env->FindClass("com/dicydev/engine/input/Input"), inputMethods.data(), inputMethods.size());

    const std::array<JNINativeMethod, 1> nativeRegistryMethods = {{
        {const_cast<char*>("getComponentBuffer"), const_cast<char*>("(JII)Ljava/nio/ByteBuffer;"), reinterpret_cast<void*>(nativeRegistryGetComponentBuffer)},
    }};
    env->RegisterNatives(env->FindClass("com/dicydev/engine/components/NativeRegistry"), nativeRegistryMethods.data(), nativeRegistryMethods.size());

    const std::array<JNINativeMethod, 1> sceneMethods = {{
        {const_cast<char*>("getCamera"), const_cast<char*>("()Lcom/dicydev/engine/scene/camera/Camera;"), reinterpret_cast<void*>(sceneGetCamera)},
    }};
    env->RegisterNatives(env->FindClass("com/dicydev/engine/scene/Scene"), sceneMethods.data(), sceneMethods.size());

    const std::array<JNINativeMethod, 2> cameraMethods = {{
        {const_cast<char*>("getPosition"), const_cast<char*>("(J)Ljava/nio/ByteBuffer;"), reinterpret_cast<void*>(cameraGetPosition)},
        {const_cast<char*>("getRotation"), const_cast<char*>("(J)Ljava/nio/ByteBuffer;"), reinterpret_cast<void*>(cameraGetRotation)},
    }};
    env->RegisterNatives(env->FindClass("com/dicydev/engine/scene/camera/Camera"), cameraMethods.data(), cameraMethods.size());
}
