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

    std::vector<std::string> arguments;
    arguments.emplace_back("-Djava.class.path=../scripting/build/classes/java/main/");
    arguments.emplace_back("-DXcheck:jni:pedantic");
    arguments.emplace_back("--enable-preview");

    if (const char* jvm_args = std::getenv("JVM_ARGS")) {
        std::istringstream stream(jvm_args);
        std::string arg;
        while (std::getline(stream, arg, ' ')) {
            if (!arg.empty()) {
                arguments.emplace_back(arg);
            }
        }
    }

    std::vector<JavaVMOption> options;
    for (auto& argument : arguments) {
        options.push_back({const_cast<char*>(argument.c_str())});
    }

    JavaVMInitArgs init_args;
    init_args.version = JNI_VERSION_21;
    init_args.nOptions = options.size();
    init_args.options = options.data();
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
    return this->new_instance("()V");
}

jobject JavaClass::new_instance(const char* signature, ...) const {
    va_list args;
    va_start(args, signature);
    const jmethodID constructor = env->GetMethodID(this->java_class, "<init>", signature);
    // env->NewGlobalRef
    const jobject instance = env->NewObjectV(this->java_class, constructor, args);
    va_end(args);
    this->check_and_clear_exceptions();
    return instance;
}

#define CALL_METHOD_IMPLEMENTATION(return_type, class_type, method_name)                                                                                                           \
    return_type JavaClass::method_name(const jobject instance, const jmethodID method, ...) const {                                                                                \
        va_list args;                                                                                                                                                              \
        va_start(args, method);                                                                                                                                                    \
        const return_type result = env->Call##class_type##MethodV(instance, method, args);                                                                                         \
        va_end(args);                                                                                                                                                              \
        this->check_and_clear_exceptions();                                                                                                                                        \
        return result;                                                                                                                                                             \
    }                                                                                                                                                                              \
    return_type JavaClass::method_name(const jmethodID method, ...) const {                                                                                                        \
        va_list args;                                                                                                                                                              \
        va_start(args, method);                                                                                                                                                    \
        const return_type result = env->CallStatic##class_type##MethodV(this->java_class, method, args);                                                                           \
        va_end(args);                                                                                                                                                              \
        this->check_and_clear_exceptions();                                                                                                                                        \
        return result;                                                                                                                                                             \
    }

void JavaClass::call_void(const jobject instance, const jmethodID method, ...) const {
    va_list args;
    va_start(args, method);
    env->CallVoidMethodV(instance, method, args);
    va_end(args);
    this->check_and_clear_exceptions();
}

void JavaClass::call_void(const jmethodID method, ...) const {
    va_list args;
    va_start(args, method);
    env->CallStaticVoidMethodV(this->java_class, method, args);
    va_end(args);
    this->check_and_clear_exceptions();
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

void JavaClass::check_and_clear_exceptions() const {
    if (!this->env->ExceptionCheck()) {
        return;
    }

    const jthrowable exception = this->env->ExceptionOccurred();
    this->env->ExceptionClear();

    const jclass exception_class = this->env->GetObjectClass(exception);
    const jmethodID to_string = this->env->GetMethodID(exception_class, "toString", "()Ljava/lang/String;");

    const auto exception_message = static_cast<jstring>(this->env->CallObjectMethod(exception, to_string));
    const char* native_string = this->env->GetStringUTFChars(exception_message, nullptr);
    DE_ERROR("Java Exception: {0}", native_string);

    this->env->ReleaseStringUTFChars(exception_message, native_string);
    this->env->DeleteLocalRef(exception_message);
    this->env->DeleteLocalRef(exception_class);
    this->env->DeleteLocalRef(exception);
}
