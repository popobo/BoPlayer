//
// Created by 33136 on 2020-4-6.
//

#ifndef BOPLAY_IPLAYER_H
#define BOPLAY_IPLAYER_H


#include "XThread.h"

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

    IDemux *demux = nullptr;
    IDecode *videoDecoder = nullptr;
    IDecode *audioDecoder = nullptr;
    IResample *resample = nullptr;
    IVideoView *videoView = nullptr;
    IAudioPlay *audioPlay = nullptr;

protected:
    IPlayer();
};


#endif //BOPLAY_IPLAYER_H
