#include <jni.h>
#include <string>
#include "FFDemux.h"
#include "XLog.h"
#include "IDecode.h"
#include "FFDecode.h"
#include "XEGL.h"
#include "XShader.h"
#include "IVideoView.h"
#include "GLVideoView.h"
#include "IResample.h"
#include "FFResample.h"
#include "IAudioPlay.h"
#include "SLAudioPlay.h"
#include <android/native_window_jni.h>


class testObs:public IObserver{
public:
    virtual void update(XData xData){
//        XLOGI("testObs update data size %d", xData.size);
    }
};

extern "C"
JNIEXPORT
jint JNI_OnLoad(JavaVM *vm, void *res){
    //初始化硬件编码接口
    FFDecode::initHard(vm);
    return JNI_VERSION_1_6;
}

IVideoView *view = nullptr;

extern "C" JNIEXPORT jstring JNICALL
//这个函数可能被调用两次导致声音重音
Java_com_bo_boplay_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";

    ///////////////////////
    //测试用代码
    testObs *tObs = new testObs();
    IDemux *demux = new FFDemux();
//    demux->addObs(tObs);
    demux->Open("/sdcard/1080.mp4");

    IDecode *vdecode = new FFDecode();
    vdecode->open(demux->getVPara(), true);

    IDecode
    *adecode = new FFDecode();
    adecode->open(demux->getAPara());

    //添加观察者
    demux->addObs(vdecode);
    demux->addObs(adecode);
    view = new GLVideoView();
    vdecode->addObs(view);

    IResample *resample = new FFResample();
    XParameter outPara = demux->getAPara();
    resample->open(demux->getAPara(), outPara);
    adecode->addObs(resample);

    IAudioPlay *audioPlay = new SLAudioPlay();
    audioPlay->startPlay(outPara);
    resample->addObs(audioPlay);

    demux->start();
    vdecode->start();
    adecode->start();

    //    XSleep(3000);
//    demux->stop();
    //    for(;;){
//        XData xData = de->Read();
//        XLOGI("Read data size is %d", xData.size);
//    }

    ///////////////////////
    return env->NewStringUTF(hello.c_str());
}
extern "C"
JNIEXPORT void JNICALL
Java_com_bo_boplay_XPlay_initView(JNIEnv *env, jobject thiz, jobject surface) {
    ANativeWindow *win = ANativeWindow_fromSurface(env,surface);
    view->setRender(win);
    //XEGL::get()->init(win);
    //XShader shader;
    //shader.init();
}