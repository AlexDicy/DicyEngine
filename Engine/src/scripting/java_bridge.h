#pragma once
#include <jni.h>


class JavaBridge {
public:
    JavaBridge();
    ~JavaBridge();

    static Ref<JavaBridge> get_instance();

    static JNIEnv* get_env() {
        return get_instance()->env;
    }

private:
    JavaVM* jvm;
    JNIEnv* env;

    typedef int(JNICALL* CreateJavaVM)(JavaVM** jvm, JNIEnv** env, JavaVMInitArgs* args);

#ifdef DE_PLATFORM_WINDOWS
    CreateJavaVM load_jvm_dll();

    HMODULE jvm_dll;
#endif
};

class JavaClass {
public:
    explicit JavaClass(const std::string& class_name);
    ~JavaClass() {
        env->DeleteGlobalRef(java_class);
    }

    jmethodID get_method(const char* method_name, const char* signature) const;
    jmethodID get_static_method(const char* method_name, const char* signature) const;

    jobject new_instance() const;

#define CALL_METHOD_DECLARATION(return_type, method_name) \
    return_type method_name(jobject instance, jmethodID method, ...) const; \
    return_type method_name(jmethodID method, ...) const;

    CALL_METHOD_DECLARATION(void, call_void)
    CALL_METHOD_DECLARATION(jboolean, call_boolean)
    CALL_METHOD_DECLARATION(jbyte, call_byte)
    CALL_METHOD_DECLARATION(jchar, call_char)
    CALL_METHOD_DECLARATION(jshort, call_short)
    CALL_METHOD_DECLARATION(jint, call_int)
    CALL_METHOD_DECLARATION(jlong, call_long)
    CALL_METHOD_DECLARATION(jfloat, call_float)
    CALL_METHOD_DECLARATION(jdouble, call_double)
    CALL_METHOD_DECLARATION(jobject, call_object)
#undef CALL_METHOD_DECLARATION

    JNIEnv* get_env() const {
        return env;
    }

    jclass get_java_class() const {
        return java_class;
    }

private:
    void check_and_clear_exceptions() const;

    JNIEnv* env;
    jclass java_class;
};
