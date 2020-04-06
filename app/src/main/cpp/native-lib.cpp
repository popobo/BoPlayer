#include <jni.h>
#include <string>
#include "XLog.h"
#include "FFPlayerBuilder.h"
#include <android/native_window_jni.h>


static IPlayer *iPlayer = nullptr;

extern "C"
JNIEXPORT
jint JNI_OnLoad(JavaVM *vm, void *res){
    //初始化硬件编码接口
    //FFDecode::initHard(vm);

    ///////////////////////
    FFPlayerBuilder::initHard(vm);
    iPlayer = FFPlayerBuilder::get()->builderPlayer();

    iPlayer->open("/sdcard/1080.mp4");
    iPlayer->start();
    ///////////////////////

    return JNI_VERSION_1_6;
}

extern "C" JNIEXPORT jstring JNICALL
//这个函数可能被调用两次导致声音重音
Java_com_bo_boplay_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";


    return env->NewStringUTF(hello.c_str());
}
extern "C"
JNIEXPORT void JNICALL
Java_com_bo_boplay_XPlay_initView(JNIEnv *env, jobject thiz, jobject surface) {
    ANativeWindow *win = ANativeWindow_fromSurface(env,surface);
    if (iPlayer){
        iPlayer->initView(win);
    }
    //    view->setRender(win);
    //XEGL::get()->init(win);
    //XShader shader;
    //shader.init();
}