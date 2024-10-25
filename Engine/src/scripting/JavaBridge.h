#pragma once
#include <jni.h>


class JavaBridge {
public:
    JavaBridge();
    ~JavaBridge();

    static Ref<JavaBridge> getInstance();

    static JNIEnv* getEnv() {
        return getInstance()->env;
    }

private:
    JavaVM* jvm;
    JNIEnv* env;

    typedef int(JNICALL* CreateJavaVM)(JavaVM** jvm, JNIEnv** env, JavaVMInitArgs* args);

#ifdef DE_PLATFORM_WINDOWS
    CreateJavaVM loadJVMdll();

    HMODULE jvmDll;
#endif
};

class JavaClass {
public:
    explicit JavaClass(const std::string& className);
    ~JavaClass() = default;

    jmethodID getMethod(const char* methodName, const char* signature) const;
    jmethodID getStaticMethod(const char* methodName, const char* signature) const;

    jobject newInstance() const;
    jobject newInstance(const char* signature, ...) const;

#define CALL_METHOD_DECLARATION(returnType, methodName) \
    returnType methodName(jobject instance, jmethodID method, ...) const; \
    returnType methodName(jmethodID method, ...) const;

    CALL_METHOD_DECLARATION(void, callVoid)
    CALL_METHOD_DECLARATION(jboolean, callBoolean)
    CALL_METHOD_DECLARATION(jbyte, callByte)
    CALL_METHOD_DECLARATION(jchar, callChar)
    CALL_METHOD_DECLARATION(jshort, callShort)
    CALL_METHOD_DECLARATION(jint, callInt)
    CALL_METHOD_DECLARATION(jlong, callLong)
    CALL_METHOD_DECLARATION(jfloat, callFloat)
    CALL_METHOD_DECLARATION(jdouble, callDouble)
    CALL_METHOD_DECLARATION(jobject, callObject)
#undef CALL_METHOD_DECLARATION

    JNIEnv* getEnv() const {
        return env;
    }

    jclass getJavaClass() const {
        return javaClass;
    }

private:
    void checkAndClearExceptions() const;

    JNIEnv* env;
    jclass javaClass;
};
