//
// Created by 33136 on 2020-4-6.
//

#ifndef BOPLAY_IPLAYER_H
#define BOPLAY_IPLAYER_H


#include "XThread.h"
#include "XParameter.h"
#include <mutex>

//用户在调用IPlay时不用关注各个模块的头文件, 只需要提供给用户一个IPlay头文件
class IDemux;
class IDecode;
class IResample;
class IVideoView;
class IAudioPlay;

class IPlayer : public XThread {
public:
    static IPlayer *get(unsigned char index = 0);
    virtual bool open(const char *url);
    virtual bool start();
    virtual void initView(void *win);
    virtual void close();

    //获取当前的播放进度 0.0-1.0
    virtual double playPos();
    virtual bool seek(double pos);
    virtual void setPause(bool isPauseIn);

    IDemux *demux = nullptr;
    IDecode *videoDecoder = nullptr;
    IDecode *audioDecoder = nullptr;
    IResample *resample = nullptr;
    IVideoView *videoView = nullptr;
    IAudioPlay *audioPlay = nullptr;

    //是否硬解码
    bool isHardDecoder = true;
    //音频输出参数配置, 这边不是使用指针必须引入头文件
    XParameter xParameterOut;
protected:
    //用作音视频同步
    void main();
    std::mutex mux;
    IPlayer(){}
};


#endif //BOPLAY_IPLAYER_H
