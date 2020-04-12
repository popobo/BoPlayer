#include <jni.h>
#include <string>
#include "XLog.h"
#include "FFPlayerBuilder.h"
#include <android/native_window_jni.h>
#include "IPlayerProxy.h"

extern "C"
JNIEXPORT
jint JNI_OnLoad(JavaVM *vm, void *res){

    IPlayerProxy::get()->init(vm);

    return JNI_VERSION_1_6;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_bo_boplay_XPlay_initView(JNIEnv *env, jobject thiz, jobject surface) {
    ANativeWindow *win = ANativeWindow_fromSurface(env,surface);
    IPlayerProxy::get()->initView(win);
}


extern "C"
JNIEXPORT void JNICALL
Java_com_bo_boplay_OpenUrl_open(JNIEnv *env, jobject thiz, jstring url) {

    const char *curl = env->GetStringUTFChars(url, 0);

    IPlayerProxy::get()->open(curl);

    IPlayerProxy::get()->start();

    env->ReleaseStringUTFChars(url, curl);
}