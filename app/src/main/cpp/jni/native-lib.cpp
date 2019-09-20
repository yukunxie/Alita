#include <jni.h>
#include <string>

#include "../render/RealRenderer.h"

extern "C" JNIEXPORT jstring JNICALL
Java_com_realxie_vulkandemo_MainActivity_stringFromJNI(JNIEnv *env, jobject /* this */)
{
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
