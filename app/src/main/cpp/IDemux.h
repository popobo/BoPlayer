//
// Created by 33136 on 2020-4-4.
//

#ifndef BOPLAY_IDEMUX_H
#define BOPLAY_IDEMUX_H

#include "XData.h"
#include "XThread.h"

//解封装接口类
class IDemux: public XThread {
public:
    //打开文件或者流媒体 rtmp http rtsp
    virtual bool Open(const char *url) = 0;

    //读取一帧数据,数据由调用者清理
    virtual XData Read() = 0;

    //总时长(单位ms)
    int totalMs = 0;
protected:
    //不要让用户访问
    virtual void main();
};


#endif //BOPLAY_IDEMUX_H