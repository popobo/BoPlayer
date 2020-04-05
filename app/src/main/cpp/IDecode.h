//
// Created by 33136 on 2020-4-5.
//

#ifndef BOPLAY_IDECODE_H
#define BOPLAY_IDECODE_H
#include "XParameter.h"
#include "IObserver.h"
#include <list>

//解码接口, 支持硬解码
class IDecode : public IObserver {
public:
    //打开解码器
    virtual bool open(XParameter parameter) = 0;

    //future模型 发送数据到线程解码
    virtual bool sendPacket(XData pkt) = 0;

    //从线程中获取解码结果, 并不会阻塞, 再次调用会复用上次空间, 线程不安全
    virtual XData recvFrame() = 0;

    //由主体notift的数据 达到最大队列缓冲则阻塞
    virtual void update(XData xData);

    bool isAudio = false;

    //最大的队列缓冲
    int maxList = 100;
protected:
    virtual void main();
    //读取缓冲
    std::list<XData> xDataList;
    std::mutex xDataListMutex;
};


#endif //BOPLAY_IDECODE_H
