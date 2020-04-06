//
// Created by 33136 on 2020-4-5.
//

#ifndef BOPLAY_FFDECODE_H
#define BOPLAY_FFDECODE_H

#include "IDecode.h"

struct AVCodecContext;
struct AVFrame;

class FFDecode : public IDecode {
public:
    static void initHard(void *vm);

    virtual bool open(XParameter parameter, bool isHard = false);

    //future模型 发送数据到线程解码
    virtual bool sendPacket(XData pkt);

    //从线程中获取解码结果, 并不会阻塞, 再次调用会复用上次空间, 线程不安全
    virtual XData recvFrame();

protected:
    AVCodecContext *codecContext = 0;
    AVFrame *frame = 0;
};


#endif //BOPLAY_FFDECODE_H
