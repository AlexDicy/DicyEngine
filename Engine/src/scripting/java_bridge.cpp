#include "pch/enginepch.h"
#include "java_bridge.h"


Ref<JavaBridge> JavaBridge::get_instance() {
    static auto instance = std::make_shared<JavaBridge>();
    return instance;
}

JavaBridge::JavaBridge() {
#ifdef DE_PLATFORM_WINDOWS
    const CreateJavaVM create_java_vm = load_jvm_dll();
#endif

    constexpr int num_options = 2;
    JavaVMOption options[num_options];
    options[0].optionString = const_cast<char*>("-Djava.class.path=../scripting/build/classes/java/main/");
    options[1].optionString = const_cast<char*>("-DXcheck:jni:pedantic");

    JavaVMInitArgs init_args;
    init_args.version = JNI_VERSION_21;
    init_args.nOptions = num_options;
    init_args.options = options;
    init_args.ignoreUnrecognized = false;

    if (const jint result = create_java_vm(&jvm, &env, &init_args); result != JNI_OK) {
        throw std::runtime_error("Failed to create Java VM, error code: " + std::to_string(result));
    }
}

JavaBridge::~JavaBridge() {
    jvm->DestroyJavaVM();
#ifdef DE_PLATFORM_WINDOWS
    FreeLibrary(jvm_dll);
#endif
}

#ifdef DE_PLATFORM_WINDOWS
JavaBridge::CreateJavaVM JavaBridge::load_jvm_dll() {
    const std::string dll_path = std::string(DE_JAVA_HOME) + R"(\bin\server\jvm.dll)";
    this->jvm_dll = LoadLibrary(dll_path.c_str());
    if (this->jvm_dll == nullptr) {
        const DWORD last_error_code = GetLastError();
        throw std::runtime_error("Failed to load jvm.dll, error code: " + std::to_string(last_error_code));
    }
    return reinterpret_cast<CreateJavaVM>(GetProcAddress(this->jvm_dll, "JNI_CreateJavaVM"));
}
#endif


JavaClass::JavaClass(const std::string& class_name) {
    this->env = JavaBridge::get_env();
    // const jclass local_java_class = env->FindClass(class_name.c_str());
    // this->java_class = static_cast<jclass>(env->NewGlobalRef(local_java_class));
    // env->DeleteLocalRef(local_java_class);
    this->java_class = env->FindClass(class_name.c_str());
}

jmethodID JavaClass::get_method(const char* method_name, const char* signature) const {
    return env->GetMethodID(this->java_class, method_name, signature);
}

jmethodID JavaClass::get_static_method(const char* method_name, const char* signature) const {
    return env->GetStaticMethodID(this->java_class, method_name, signature);
}

jobject JavaClass::new_instance() const {
    const jmethodID constructor = env->GetMethodID(this->java_class, "<init>", "()V");
    // env->NewGlobalRef
    return env->NewObject(this->java_class, constructor);
}

#define CALL_METHOD_IMPLEMENTATION(return_type, class_type, method_name)                                                                                                           \
    return_type JavaClass::method_name(jobject instance, jmethodID method, ...) const {                                                                                            \
        va_list args;                                                                                                                                                              \
        va_start(args, method);                                                                                                                                                    \
        const return_type result = env->Call##class_type##MethodV(instance, method, args);                                                                                         \
        va_end(args);                                                                                                                                                              \
        return result;                                                                                                                                                             \
    }                                                                                                                                                                              \
    return_type JavaClass::method_name(jmethodID method, ...) const {                                                                                                              \
        va_list args;                                                                                                                                                              \
        va_start(args, method);                                                                                                                                                    \
        const return_type result = env->CallStatic##class_type##MethodV(this->java_class, method, args);                                                                           \
        va_end(args);                                                                                                                                                              \
        return result;                                                                                                                                                             \
    }

void JavaClass::call_void(jobject instance, jmethodID method, ...) const {
    va_list args;
    va_start(args, method);
    env->CallVoidMethodV(instance, method, args);
    va_end(args);
}

void JavaClass::call_void(jmethodID method, ...) const {
    va_list args;
    va_start(args, method);
    env->CallStaticVoidMethodV(this->java_class, method, args);
    va_end(args);
}

CALL_METHOD_IMPLEMENTATION(jboolean, Boolean, call_boolean)
CALL_METHOD_IMPLEMENTATION(jbyte, Byte, call_byte)
CALL_METHOD_IMPLEMENTATION(jchar, Char, call_char)
CALL_METHOD_IMPLEMENTATION(jshort, Short, call_short)
CALL_METHOD_IMPLEMENTATION(jint, Int, call_int)
CALL_METHOD_IMPLEMENTATION(jlong, Long, call_long)
CALL_METHOD_IMPLEMENTATION(jfloat, Float, call_float)
CALL_METHOD_IMPLEMENTATION(jdouble, Double, call_double)
CALL_METHOD_IMPLEMENTATION(jobject, Object, call_object)

#undef CALL_METHOD_IMPLEMENTATION
