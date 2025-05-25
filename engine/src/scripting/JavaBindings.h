#pragma once

struct JNIEnv_;
typedef JNIEnv_ JNIEnv;
class _jobject;
typedef _jobject* jobject;

class JavaBindings {
public:
    static void init();

private:
    // methods that require to be in the class to be able to access protected members of friend classes
    static void transformInvalidateGlobal(JNIEnv* env, jobject, jobject directBuffer);
};
