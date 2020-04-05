#include <jni.h>
#include <string>
#include "FFDemux.h"
#include "XLog.h"
#include "IDecode.h"
#include "FFDecode.h"

class testObs:public IObserver{
public:
    virtual void update(XData xData){
//        XLOGI("testObs update data size %d", xData.size);
    }
};

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
    demux->Open("/sdcard/test.mp4");

    IDecode *vdecode = new FFDecode();
    vdecode->open(demux->getVPara());

    IDecode *adecode = new FFDecode();
    adecode->open(demux->getAPara());

    //添加观察者
    demux->addObs(vdecode);
    demux->addObs(adecode);

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
