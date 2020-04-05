#include <jni.h>
#include <string>
#include "FFDemux.h"
#include "XLog.h"

class testObs:public IObserver{
public:
    virtual void update(XData xData){
        XLOGI("testObs update data size %d", xData.size);
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
    IDemux *de = new FFDemux();
    de->addObs(tObs);
    de->Open("/sdcard/test.mp4");
    de->start();
    XSleep(3000);
    de->stop();
    //    for(;;){
//        XData xData = de->Read();
//        XLOGI("Read data size is %d", xData.size);
//    }

    ///////////////////////
    return env->NewStringUTF(hello.c_str());
}
