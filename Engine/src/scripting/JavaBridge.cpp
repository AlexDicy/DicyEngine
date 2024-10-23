#include "pch/enginepch.h"
#include "JavaBridge.h"


Ref<JavaBridge> JavaBridge::getInstance() {
    static auto instance = std::make_shared<JavaBridge>();
    return instance;
}

JavaBridge::JavaBridge() {
#ifdef DE_PLATFORM_WINDOWS
    const CreateJavaVM createJavaVM = loadJVMdll();
#else
    const auto create_java_vm = reinterpret_cast<CreateJavaVM>(JNI_CreateJavaVM);
#endif

    std::vector<std::string> arguments;
    arguments.emplace_back("-Djava.class.path=../scripting/build/classes/java/main/");
    arguments.emplace_back("-DXcheck:jni:pedantic");
    arguments.emplace_back("--enable-preview");

    if (const char* jvmArgs = std::getenv("JVM_ARGS")) {
        std::istringstream stream(jvmArgs);
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

    JavaVMInitArgs initArgs;
    initArgs.version = JNI_VERSION_21;
    initArgs.nOptions = options.size();
    initArgs.options = options.data();
    initArgs.ignoreUnrecognized = false;

    if (const jint result = createJavaVM(&jvm, &env, &initArgs); result != JNI_OK) {
        throw std::runtime_error("Failed to create Java VM, error code: " + std::to_string(result));
    }
}

JavaBridge::~JavaBridge() {
    jvm->DestroyJavaVM();
#ifdef DE_PLATFORM_WINDOWS
    FreeLibrary(jvmDll);
#endif
}

#ifdef DE_PLATFORM_WINDOWS
JavaBridge::CreateJavaVM JavaBridge::loadJVMdll() {
    const std::string dllPath = std::string(DE_JAVA_HOME) + R"(\bin\server\jvm.dll)";
    this->jvmDll = LoadLibrary(dllPath.c_str());
    if (this->jvmDll == nullptr) {
        const DWORD lastErrorCode = GetLastError();
        throw std::runtime_error("Failed to load jvm.dll, error code: " + std::to_string(lastErrorCode));
    }
    return reinterpret_cast<CreateJavaVM>(GetProcAddress(this->jvmDll, "JNI_CreateJavaVM"));
}
#endif


JavaClass::JavaClass(const std::string& className) {
    this->env = JavaBridge::getEnv();
    // const jclass local_java_class = env->FindClass(class_name.c_str());
    // this->java_class = static_cast<jclass>(env->NewGlobalRef(local_java_class));
    // env->DeleteLocalRef(local_java_class);
    this->javaClass = env->FindClass(className.c_str());
}

jmethodID JavaClass::getMethod(const char* methodName, const char* signature) const {
    return env->GetMethodID(this->javaClass, methodName, signature);
}

jmethodID JavaClass::getStaticMethod(const char* methodName, const char* signature) const {
    return env->GetStaticMethodID(this->javaClass, methodName, signature);
}

jobject JavaClass::newInstance() const {
    return this->newInstance("()V");
}

jobject JavaClass::newInstance(const char* signature, ...) const {
    va_list args;
    va_start(args, signature);
    const jmethodID constructor = env->GetMethodID(this->javaClass, "<init>", signature);
    // env->NewGlobalRef
    const jobject instance = env->NewObjectV(this->javaClass, constructor, args);
    va_end(args);
    this->checkAndClearExceptions();
    return instance;
}

#define CALL_METHOD_IMPLEMENTATION(returnType, classType, methodName)                                                                                                              \
    returnType JavaClass::methodName(const jobject instance, const jmethodID method, ...) const {                                                                                  \
        va_list args;                                                                                                                                                              \
        va_start(args, method);                                                                                                                                                    \
        const returnType result = env->Call##classType##MethodV(instance, method, args);                                                                                           \
        va_end(args);                                                                                                                                                              \
        this->checkAndClearExceptions();                                                                                                                                           \
        return result;                                                                                                                                                             \
    }                                                                                                                                                                              \
    returnType JavaClass::methodName(const jmethodID method, ...) const {                                                                                                          \
        va_list args;                                                                                                                                                              \
        va_start(args, method);                                                                                                                                                    \
        const returnType result = env->CallStatic##classType##MethodV(this->javaClass, method, args);                                                                              \
        va_end(args);                                                                                                                                                              \
        this->checkAndClearExceptions();                                                                                                                                           \
        return result;                                                                                                                                                             \
    }

void JavaClass::callVoid(const jobject instance, const jmethodID method, ...) const {
    va_list args;
    va_start(args, method);
    env->CallVoidMethodV(instance, method, args);
    va_end(args);
    this->checkAndClearExceptions();
}

void JavaClass::callVoid(const jmethodID method, ...) const {
    va_list args;
    va_start(args, method);
    env->CallStaticVoidMethodV(this->javaClass, method, args);
    va_end(args);
    this->checkAndClearExceptions();
}

CALL_METHOD_IMPLEMENTATION(jboolean, Boolean, callBoolean)
CALL_METHOD_IMPLEMENTATION(jbyte, Byte, callByte)
CALL_METHOD_IMPLEMENTATION(jchar, Char, callChar)
CALL_METHOD_IMPLEMENTATION(jshort, Short, callShort)
CALL_METHOD_IMPLEMENTATION(jint, Int, callInt)
CALL_METHOD_IMPLEMENTATION(jlong, Long, callLong)
CALL_METHOD_IMPLEMENTATION(jfloat, Float, callFloat)
CALL_METHOD_IMPLEMENTATION(jdouble, Double, callDouble)
CALL_METHOD_IMPLEMENTATION(jobject, Object, callObject)

#undef CALL_METHOD_IMPLEMENTATION

void JavaClass::checkAndClearExceptions() const {
    if (!this->env->ExceptionCheck()) {
        return;
    }

    const jthrowable exception = this->env->ExceptionOccurred();
    this->env->ExceptionClear();

    const jclass exceptionClass = this->env->GetObjectClass(exception);
    const jmethodID toString = this->env->GetMethodID(exceptionClass, "toString", "()Ljava/lang/String;");

    const auto exceptionMessage = static_cast<jstring>(this->env->CallObjectMethod(exception, toString));
    const char* nativeString = this->env->GetStringUTFChars(exceptionMessage, nullptr);
    DE_ERROR("Java Exception: {0}", nativeString);

    this->env->ReleaseStringUTFChars(exceptionMessage, nativeString);
    this->env->DeleteLocalRef(exceptionMessage);
    this->env->DeleteLocalRef(exceptionClass);
    this->env->DeleteLocalRef(exception);
}
