#include <jni.h>
#include <string>
#include <android/asset_manager_jni.h>

#include "../render/RealRenderer.h"
#include "../aux/AFileSystem.h"

extern "C" {

JNIEXPORT jstring JNICALL
Java_com_realxie_vulkandemo_MainActivity_stringFromJNI(JNIEnv *env, jobject /* this */)
{
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

JNIEXPORT void JNICALL
Java_com_realxie_vulkandemo_MainActivity_setAssetManagerJNI(JNIEnv *env, jobject thiz,
                                                          jobject context, jobject assetManager)
{
    AFileSystem::getInstance()->initAssetManager(AAssetManager_fromJava(env, assetManager));
}

}
