//
// Created by 33136 on 2020-4-6.
//

#include "IAudioPlay.h"
#include "XLog.h"

void IAudioPlay::update(XData xData) {
//    XLOGI("IAudioPlay::update xData size = %d", xData.size);
    //压入缓冲队列
    if (xData.size <= 0 || !xData.data){
        return;
    }
    while(!isExit){

        framesMutex.lock();
        if (frames.size() > maxFrame){
            framesMutex.unlock();
            XSleep(1);
            continue;
        }
        frames.push_back(xData);
        framesMutex.unlock();
        break;
    }
}

XData IAudioPlay::getData() {
    XData xData;
    while (!isExit){
        framesMutex.lock();
        if (!frames.empty()){
            xData = frames.front();
            frames.pop_front();
            framesMutex.unlock();
            return xData;
        }
        framesMutex.unlock();
        XSleep(1);
    }
    return xData;
}