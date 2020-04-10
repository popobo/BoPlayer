//
// Created by 33136 on 2020-4-6.
//

#ifndef BOPLAY_IAUDIOPLAY_H
#define BOPLAY_IAUDIOPLAY_H


#include <list>
#include "IObserver.h"
#include "XParameter.h"

class IAudioPlay : public IObserver{
public:
    //缓冲满后阻塞
    virtual void update(XData xData);
    virtual bool startPlay(XParameter xParameter) = 0;
    //获取缓冲数据, 如果没有则阻塞
    virtual XData getData();

    virtual void close() = 0;
    virtual void clear();

    //最大缓冲
    int maxFrame = 100;

    //音频时间
    int pts = 0;
protected:
    std::list<XData> frames;
    std::mutex framesMutex;
};


#endif //BOPLAY_IAUDIOPLAY_H
