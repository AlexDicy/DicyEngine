#include "pch/enginepch.h"
#include "java_bindings.h"

#include "java_bridge.h"

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

void JavaBindings::init() {
    JNIEnv* env = JavaBridge::get_env();

    const std::array<JNINativeMethod, 2> input_methods = {{
        {const_cast<char*>("setAction"), const_cast<char*>("(Ljava/lang/String;I)V"), reinterpret_cast<void*>(input_set_action)},
        {const_cast<char*>("isActionPressed"), const_cast<char*>("(Ljava/lang/String;)Z"), reinterpret_cast<void*>(input_is_action_pressed)}
    }};
    env->RegisterNatives(env->FindClass("com/dicydev/engine/input/Input"), input_methods.data(), input_methods.size());
}
