//
// Created by 33136 on 2020-4-6.
//

#include "IPlayer.h"
#include "IDemux.h"
#include "IDecode.h"
#include "IVideoView.h"
#include "IAudioPlay.h"
#include "IResample.h"
#include "XLog.h"

IPlayer *IPlayer::get(unsigned char index) {
    static IPlayer iPlayer[256];
    return &iPlayer[index];
}

bool IPlayer::open(const char *url) {
    //解封装
    mux.lock();
    if(!demux || !demux->open(url)){
        mux.unlock();
        XLOGE("demux->open %s failed!", url);
        return false;
    }
    if (!videoDecoder || !videoDecoder->open(demux->getVPara(), isHardDecoder)){
        XLOGE("videoDecoder->open failed!");
        //因为数据不一定需要解码, 所以不直接return, 如果解封之后就是原始数据
//        return false;
    }

    if (!audioDecoder || !audioDecoder->open(demux->getAPara())){
        XLOGE("audioDecoder->open failed!");
    }
    //重采样有可能不需要, 解码后或者解封装后可能是直接能播放的数据
    if (xParameterOut.sample_rate <= 0){
        xParameterOut = demux->getAPara();
    }
    if (!resample || !resample->open(demux->getAPara(), xParameterOut)){
        XLOGE("resample->open failed!");
    }
    mux.unlock();
    return true;
}

bool IPlayer::start() {
    mux.lock();
    if (!demux || !demux->start()){
        //每个return语句都要有锁的释放
        mux.unlock();
        XLOGE("demux->start failed");
        return false;
    }
    if (audioDecoder){
        audioDecoder->start();
    }
    if (audioPlay){
        audioPlay->startPlay(xParameterOut);
    }
    if (videoDecoder){
        videoDecoder->start();
    }

    mux.unlock();
    XThread::start();
    return true;
}

void IPlayer::initView(void *win) {
    if (videoView){
        videoView->setRender(win);
    }
}

void IPlayer::main() {
    while(!isExit){
        mux.lock();

        if (!audioPlay || !videoDecoder){
            //等待初始化完成再进行同步
            mux.unlock();
            XSleep(1);
            continue;
        }

        //同步
        //获取音频的pts 告诉视频
        videoDecoder->synPts = audioPlay->pts;

        mux.unlock();
        XSleep(1);
    }
}
