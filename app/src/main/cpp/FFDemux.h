//
// Created by 33136 on 2020-4-4.
//

#ifndef BOPLAY_FFDEMUX_H
#define BOPLAY_FFDEMUX_H

#include "IDemux.h"
//声明, 不引用头文件, 封装后调用者不需要知道ffmpeg的头文件
struct AVFormatContext;

class FFDemux: public IDemux{
public:
    //打开文件或者流媒体 rtmp http rtsp
    virtual bool open(const char *url);

    //获取视频参数
    virtual XParameter getVPara();
    //获取音频频参数
    virtual XParameter getAPara();

    //读取一帧数据,数据由调用者清理
    virtual XData Read();

    FFDemux();

private:
    //只有在无参数构造函数时, 这个赋值才会生效
    AVFormatContext *ic = 0;

    int audioStream = 1;
    int videoStream = 0;
};


#endif //BOPLAY_FFDEMUX_H
