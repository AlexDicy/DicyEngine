#include "pch/enginepch.h"
#include "java_bindings.h"

#include "java_bridge.h"
#include "scene/scene.h"

#include <entt/entity/registry.hpp>

void input_set_action(JNIEnv* env, jobject, const jstring action, jint key) {
    const char* action_str = env->GetStringUTFChars(action, nullptr);
    Input::set_action(action_str, static_cast<InputCode>(key));
    env->ReleaseStringUTFChars(action, action_str);
}

jboolean input_is_action_pressed(JNIEnv* env, jobject, const jstring action) {
    const char* action_str = env->GetStringUTFChars(action, nullptr);
    const jboolean result = Input::is_action_pressed(action_str);
    env->ReleaseStringUTFChars(action, action_str);
    return result;
}

void input_bind_axis(JNIEnv* env, jobject, const jstring axis, const jobject callback) {
    const char* axis_str = env->GetStringUTFChars(axis, nullptr);
    const auto callback_ref = env->NewGlobalRef(callback);

    Input::bind_axis(axis_str, [env, callback_ref](const float delta) {
        env->CallVoidMethod(callback_ref, env->GetMethodID(env->GetObjectClass(callback_ref), "callback", "(F)V"), delta);
    });

    env->ReleaseStringUTFChars(axis, axis_str);
}

void input_bind_action_pressed(JNIEnv* env, jobject, const jstring action, const jobject callback) {
    const char* action_str = env->GetStringUTFChars(action, nullptr);
    const auto callback_ref = env->NewGlobalRef(callback);

    Input::bind_action_pressed(action_str, [env, callback_ref] {
        env->CallVoidMethod(callback_ref, env->GetMethodID(env->GetObjectClass(callback_ref), "callback", "()V"));
    });

    env->ReleaseStringUTFChars(action, action_str);
}

void input_bind_action_released(JNIEnv* env, jobject, const jstring action, const jobject callback) {
    const char* action_str = env->GetStringUTFChars(action, nullptr);
    const auto callback_ref = env->NewGlobalRef(callback);

    Input::bind_action_released(action_str, [env, callback_ref] {
        env->CallVoidMethod(callback_ref, env->GetMethodID(env->GetObjectClass(callback_ref), "callback", "()V"));
    });

    env->ReleaseStringUTFChars(action, action_str);
}


jobject native_registry_get_component_buffer(JNIEnv* env, jobject, const jlong registry, jint entity_id, jint component_id) {
    const auto* registry_pointer = reinterpret_cast<entt::registry*>(registry); // NOLINT(performance-no-int-to-ptr)
    const auto entity = static_cast<entt::entity>(entity_id);

    // just hard code component type to be Transform for now
    const Transform& component = registry_pointer->get<Transform>(entity);
    constexpr auto component_size = sizeof(Transform);
    return env->NewDirectByteBuffer(const_cast<Transform*>(&component), component_size);
}

jobject scene_get_camera(JNIEnv* env, const jobject scene_object) {
    const jclass scene_class = env->GetObjectClass(scene_object);
    const jlong scene_pointer = env->GetLongField(scene_object, env->GetFieldID(scene_class, "pointer", "J"));
    const auto* scene = reinterpret_cast<Scene*>(scene_pointer); // NOLINT(performance-no-int-to-ptr)
    const jobject camera_object = JavaClass("com/dicydev/engine/scene/camera/Camera").new_instance("(J)V", scene->get_camera().get());
    return camera_object;
}

jobject camera_get_position(JNIEnv* env, jobject, const jlong camera) {
    const auto* camera_pointer = reinterpret_cast<Camera*>(camera); // NOLINT(performance-no-int-to-ptr)
    const glm::vec3& position = camera_pointer->get_position();
    constexpr auto position_size = sizeof(glm::vec3);
    return env->NewDirectByteBuffer(const_cast<glm::vec3*>(&position), position_size);
}

jobject camera_get_rotation(JNIEnv* env, jobject, const jlong camera) {
    const auto* camera_pointer = reinterpret_cast<Camera*>(camera); // NOLINT(performance-no-int-to-ptr)
    const Rotation& rotation = camera_pointer->get_rotation();
    constexpr auto rotation_size = sizeof(Rotation);
    return env->NewDirectByteBuffer(const_cast<Rotation*>(&rotation), rotation_size);
}

void JavaBindings::init() {
    JNIEnv* env = JavaBridge::get_env();

    const std::array<JNINativeMethod, 5> input_methods = {{
        {const_cast<char*>("setAction"), const_cast<char*>("(Ljava/lang/String;I)V"), reinterpret_cast<void*>(input_set_action)},
        {const_cast<char*>("isActionPressed"), const_cast<char*>("(Ljava/lang/String;)Z"), reinterpret_cast<void*>(input_is_action_pressed)},
        {const_cast<char*>("bindAxis"), const_cast<char*>("(Ljava/lang/String;Lcom/dicydev/engine/input/InputAxisCallback;)V"), reinterpret_cast<void*>(input_bind_axis)},
        {const_cast<char*>("bindActionPressed"), const_cast<char*>("(Ljava/lang/String;Lcom/dicydev/engine/input/InputActionCallback;)V"), reinterpret_cast<void*>(input_bind_action_pressed)},
        {const_cast<char*>("bindActionReleased"), const_cast<char*>("(Ljava/lang/String;Lcom/dicydev/engine/input/InputActionCallback;)V"), reinterpret_cast<void*>(input_bind_action_released)},
    }};
    env->RegisterNatives(env->FindClass("com/dicydev/engine/input/Input"), input_methods.data(), input_methods.size());

    const std::array<JNINativeMethod, 1> native_registry_methods = {{
        {const_cast<char*>("getComponentBuffer"), const_cast<char*>("(JII)Ljava/nio/ByteBuffer;"), reinterpret_cast<void*>(native_registry_get_component_buffer)},
    }};
    env->RegisterNatives(env->FindClass("com/dicydev/engine/components/NativeRegistry"), native_registry_methods.data(), native_registry_methods.size());

    const std::array<JNINativeMethod, 1> scene_methods = {{
        {const_cast<char*>("getCamera"), const_cast<char*>("()Lcom/dicydev/engine/scene/camera/Camera;"), reinterpret_cast<void*>(scene_get_camera)},
    }};
    env->RegisterNatives(env->FindClass("com/dicydev/engine/scene/Scene"), scene_methods.data(), scene_methods.size());

    const std::array<JNINativeMethod, 2> camera_methods = {{
        {const_cast<char*>("getPosition"), const_cast<char*>("(J)Ljava/nio/ByteBuffer;"), reinterpret_cast<void*>(camera_get_position)},
        {const_cast<char*>("getRotation"), const_cast<char*>("(J)Ljava/nio/ByteBuffer;"), reinterpret_cast<void*>(camera_get_rotation)},
    }};
    env->RegisterNatives(env->FindClass("com/dicydev/engine/scene/camera/Camera"), camera_methods.data(), camera_methods.size());
}
