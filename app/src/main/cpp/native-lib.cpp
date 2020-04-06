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
#include <android/native_window_jni.h>


class testObs:public IObserver{
public:
    virtual void update(XData xData){
//        XLOGI("testObs update data size %d", xData.size);
    }
};

IVideoView *view = nullptr;

extern "C" JNIEXPORT jstring JNICALL
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
    vdecode->open(demux->getVPara());

    IDecode
    *adecode = new FFDecode();
    adecode->open(demux->getAPara());

    //添加观察者
    demux->addObs(vdecode);
    demux->addObs(adecode);
    view = new GLVideoView();
    vdecode->addObs(view);

    IResample *resample = new FFResample();
    resample->open(demux->getAPara());
    adecode->addObs(resample);

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