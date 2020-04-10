//
// Created by 33136 on 2020-4-6.
//

#include "SLAudioPlay.h"
#include "XLog.h"

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

//这些只有在清理时用得到, 所以用静态, 而如果变成成员变量, 则要在相应.h中添加头文件
static  SLObjectItf engineSL = nullptr;
static  SLEngineItf en = nullptr;
static  SLObjectItf mix = nullptr;
static SLObjectItf player = nullptr;
static SLPlayItf playerInterface = nullptr;
static SLAndroidSimpleBufferQueueItf pcmQueue = nullptr;

static SLEngineItf CreateSL(){
    SLresult  ret;
    SLEngineItf en;//指针
    //创建引擎
    ret = slCreateEngine(&engineSL, 0, 0, 0, 0, 0);
    if(ret != SL_RESULT_SUCCESS){
        return  nullptr;
    }

    //实例化, 传入SL_BOOLEAN_FALSE表示等待对象的创建
    ret = (*engineSL)->Realize(engineSL, SL_BOOLEAN_FALSE);
    if(ret != SL_RESULT_SUCCESS){
        return  nullptr;
    }

    //获取引擎的接口
    ret = (*engineSL)->GetInterface(engineSL, SL_IID_ENGINE, &en);
    if(ret != SL_RESULT_SUCCESS){
        return  nullptr;
    }

    //返回地址
    return  en;
}


void SLAudioPlay::playCall(void *bufQueue) {
    if (!bufQueue){
        return;
    }

    SLAndroidSimpleBufferQueueItf bufQueueGot = (SLAndroidSimpleBufferQueueItf)bufQueue;
//    XLOGI("SLAudioPlay::playCall")
    //阻塞
    XData xData = getData();
    if (xData.size <= 0){
        XLOGE("getData() size is 0");
        return;
    }
    if(!buf){
        return;
    }
    //在这边用buf存储, 是因为xData在playCall调用结束后就销毁了, 但OpenSL内部播放时还需要要用到这部分数据, 这样就避免访问已销毁地址
    memcpy(buf, xData.data, xData.size);
    mux.lock();
    (*bufQueueGot)->Enqueue(bufQueueGot, buf, xData.size);
    mux.unlock();
    xData.drop();

}

static void PcmCall(SLAndroidSimpleBufferQueueItf bf, void *contex)
{
    SLAudioPlay *slAudioPlay = (SLAudioPlay*)contex;
    if (!slAudioPlay){
        XLOGE("PcmCall failed! contex is nullptr");
        return;
    }
    slAudioPlay->playCall((void *)bf);
}

bool SLAudioPlay::startPlay(XParameter xParameter) {

    close();
    mux.lock();
    //1.创建引擎
    en = CreateSL();
    if(en){
        XLOGI("CreateSL success!");
    }else{
        XLOGE("CreateSL failed!");
    }

    //2.创建混音器
    //混音器
    SLresult ret;
    ret = (*en)->CreateOutputMix(en, &mix, 0, 0, 0);
    if (ret != SL_RESULT_SUCCESS){
        mux.unlock();
        XLOGE("(*en)->CreateOutputMix failed!");
        return false;
    }
    //实例化混音器 阻塞式地等待创建
    ret = (*mix)->Realize(mix, SL_BOOLEAN_FALSE);
    if (ret != SL_RESULT_SUCCESS){
        mux.unlock();
        XLOGE("(*en)->CreateOutputMix failed!");
        return false;
    }

    //存储一个接口给player使用
    SLDataLocator_OutputMix outputMix = {SL_DATALOCATOR_OUTPUTMIX, mix};
    //结构体
    SLDataSink audioSink = {&outputMix, 0};

    //3.配置音频信息
    //缓冲队列
    SLDataLocator_AndroidSimpleBufferQueue que = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 10};
    //音频格式
    SLDataFormat_PCM pcm = {
            SL_DATAFORMAT_PCM,
            (SLuint32)xParameter.channels,//声道数
            (SLuint32)xParameter.sample_rate * 1000,//采样率
            SL_PCMSAMPLEFORMAT_FIXED_16,//存储大小
            SL_PCMSAMPLEFORMAT_FIXED_16,
            SL_SPEAKER_FRONT_LEFT|SL_SPEAKER_FRONT_RIGHT,//左右声道
            SL_BYTEORDER_LITTLEENDIAN//字节序,小端,主机
    };
    //生成结构体给播放器使用
    SLDataSource ds = {&que, &pcm};

    //4 创建播放器
    const SLInterfaceID ids[] = {SL_IID_BUFFERQUEUE};//表示需要哪些接口
    const SLboolean req[] = {SL_BOOLEAN_TRUE};//表示这个接口是开放还是关闭
    ret = (*en)->CreateAudioPlayer(en, &player, &ds, &audioSink, sizeof(ids)/ sizeof(SLInterfaceID), ids ,req);
    if (ret != SL_RESULT_SUCCESS){
        mux.unlock();
        XLOGE("(*en)->CreateAudioPlayer failed!");
        return false;
    }else{
        XLOGI("(*en)->CreateAudioPlayer successfully!");
    }
    //实例化
    (*player)->Realize(player, SL_BOOLEAN_FALSE);
    //获取player接口
    ret = (*player)->GetInterface(player, SL_IID_PLAY, &playerInterface);
    if (ret != SL_RESULT_SUCCESS){
        mux.unlock();
        XLOGE("(*player)->GetInterface playerInterface failed!");
        return false;
    }else{
        XLOGI("(*player)->GetInterface playerInterface successfully!");
    }
    //获取队列接口
    ret = (*player)->GetInterface(player, SL_IID_BUFFERQUEUE, &pcmQueue);
    if (ret != SL_RESULT_SUCCESS){
        mux.unlock();
        XLOGE("(*player)->GetInterface pcmQueue failed!");
        return false;
    }else{
        XLOGI("(*player)->GetInterface pcmQueue successfully!");
    }

    //设置缓冲接口回调函数,播放队列空的时候调用, 第一次要先放点东西
    (*pcmQueue)->RegisterCallback(pcmQueue, PcmCall, this);
    //设置为播放状态
    (*playerInterface)->SetPlayState(playerInterface, SL_PLAYSTATE_PLAYING);
    //启动队列回调
    (*pcmQueue)->Enqueue(pcmQueue, "", 1);

    XLOGI("SLAudioPlay::startPlay successfully");
    mux.unlock();
    return true;
}

SLAudioPlay::SLAudioPlay() {
    buf = new unsigned char[1024 * 1024];
}

SLAudioPlay::~SLAudioPlay() {
    delete buf;
    buf = nullptr;
}

void SLAudioPlay::close() {
    mux.lock();
    //停止播放
    if (playerInterface && (*playerInterface)){
        (*playerInterface)->SetPlayState(playerInterface, SL_PLAYSTATE_STOPPED);
    }
    //清理播放队列
    if (pcmQueue && (*pcmQueue)){
        (*pcmQueue)->Clear(pcmQueue);
    }
    //销毁player对象
    if (player && (*player)){
        (*player)->Destroy(player);
    }
    //销毁混音器
    if (mix && (*mix)){
        (*mix)->Destroy(mix);
    }
    //销毁播放器, 播放器销毁后其对应接口也销毁了
     if (engineSL && (*engineSL)){
         (*engineSL)->Destroy(engineSL);
     }
    mux.unlock();
}

