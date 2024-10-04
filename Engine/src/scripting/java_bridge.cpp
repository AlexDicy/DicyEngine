#include "pch/enginepch.h"
#include "java_bridge.h"

#include <jni.h>

typedef int(JNICALL* CreateJavaVM)(JavaVM** jvm, JNIEnv** env, JavaVMInitArgs* args);

#ifdef DE_PLATFORM_WINDOWS
static HMODULE jvm_dll;

CreateJavaVM load_jvm_dll() {
    const std::string dll_path = std::string(DE_JAVA_HOME) + R"(\bin\server\jvm.dll)";
    jvm_dll = LoadLibrary(dll_path.c_str());
    if (jvm_dll == nullptr) {
        const DWORD last_error_code = GetLastError();
        throw std::runtime_error("Failed to load jvm.dll, error code: " + std::to_string(last_error_code));
    }
    return reinterpret_cast<CreateJavaVM>(GetProcAddress(jvm_dll, "JNI_CreateJavaVM"));
}
#endif


class JavaBridge::JavaBridgeImpl : public JavaBridge {
public:
    JavaBridgeImpl() {
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

    ~JavaBridgeImpl() override {
        jvm->DestroyJavaVM();
#ifdef DE_PLATFORM_WINDOWS
        FreeLibrary(jvm_dll);
#endif
    }

    void increment_counter() const override {
        const jclass engine_bridge = env->FindClass("com/dicydev/engine/EngineBridge");
        const jmethodID get_instance = env->GetStaticMethodID(engine_bridge, "getInstance", "()Lcom/dicydev/engine/EngineBridge;");
        const jobject instance = env->CallStaticObjectMethod(engine_bridge, get_instance);
        const jmethodID increment_counter = env->GetMethodID(engine_bridge, "incrementCounter", "()V");
        env->CallVoidMethod(instance, increment_counter);
    }

    int get_counter() const override {
        const jclass engine_bridge = env->FindClass("com/dicydev/engine/EngineBridge");
        const jmethodID get_instance = env->GetStaticMethodID(engine_bridge, "getInstance", "()Lcom/dicydev/engine/EngineBridge;");
        const jobject instance = env->CallStaticObjectMethod(engine_bridge, get_instance);
        const jmethodID get_counter = env->GetMethodID(engine_bridge, "getCounter", "()I");
        return env->CallIntMethod(instance, get_counter);
    }

private:
    JavaVM* jvm;
    JNIEnv* env;
};


Ref<JavaBridge> JavaBridge::get_instance() {
    static auto instance = std::make_shared<JavaBridgeImpl>();
    return instance;
}
