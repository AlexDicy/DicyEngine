#include "pch/enginepch.h"
#include "java_bindings.h"

#include "java_bridge.h"

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

jobject native_registry_get_component_buffer(JNIEnv* env, jobject, jlong registry, jint entity_id, jint component_id) {
    const auto* registry_pointer = reinterpret_cast<entt::registry*>(registry); // NOLINT(performance-no-int-to-ptr)
    const auto entity = static_cast<entt::entity>(entity_id);

    // just hard code component type to be Transform for now
    const Transform& component = registry_pointer->get<Transform>(entity);
    constexpr auto component_size = sizeof(Transform);
    return env->NewDirectByteBuffer(const_cast<Transform*>(&component), component_size);
}

void JavaBindings::init() {
    JNIEnv* env = JavaBridge::get_env();

    const std::array<JNINativeMethod, 2> input_methods = {{
        {const_cast<char*>("setAction"), const_cast<char*>("(Ljava/lang/String;I)V"), reinterpret_cast<void*>(input_set_action)},
        {const_cast<char*>("isActionPressed"), const_cast<char*>("(Ljava/lang/String;)Z"), reinterpret_cast<void*>(input_is_action_pressed)},
    }};
    env->RegisterNatives(env->FindClass("com/dicydev/engine/input/Input"), input_methods.data(), input_methods.size());

    const std::array<JNINativeMethod, 1> native_registry_methods = {{
        {const_cast<char*>("getComponentBuffer"), const_cast<char*>("(JII)Ljava/nio/ByteBuffer;"), reinterpret_cast<void*>(native_registry_get_component_buffer)},
    }};
    env->RegisterNatives(env->FindClass("com/dicydev/engine/components/NativeRegistry"), native_registry_methods.data(), native_registry_methods.size());
}
