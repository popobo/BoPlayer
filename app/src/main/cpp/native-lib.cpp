#include <jni.h>
#include <string>
#include "XLog.h"
#include "FFPlayerBuilder.h"
#include <android/native_window_jni.h>
#include "IPlayerProxy.h"

extern "C"
JNIEXPORT
jint JNI_OnLoad(JavaVM *vm, void *res){

    ///////////////////////
    IPlayerProxy::get()->init(vm);

    IPlayerProxy::get()->open("/sdcard/1080.mp4");
    IPlayerProxy::get()->start();
    ///////////////////////

    return JNI_VERSION_1_6;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_bo_boplay_XPlay_initView(JNIEnv *env, jobject thiz, jobject surface) {
    ANativeWindow *win = ANativeWindow_fromSurface(env,surface);
    IPlayerProxy::get()->initView(win);
}

extern "C" JNIEXPORT jstring JNICALL
//这个函数可能被调用两次导致声音重音
Java_com_bo_boplay_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";


    return env->NewStringUTF(hello.c_str());
}
