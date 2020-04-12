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

    //因为IAudioPlay没启用线程, 所以isRuning没有置true, 按照XThread::stop()逻辑, isExit无法置为true
    isRunning = true;
    while (!isExit){
        if (isPaused()){
            XSleep(1);
            continue;
        }

        framesMutex.lock();
        if (!frames.empty()){
            //有数据返回
            xData = frames.front();
            frames.pop_front();
            framesMutex.unlock();
            pts = xData.pts;
            return xData;
        }
        framesMutex.unlock();
        XSleep(1);
    }
    isRunning = false;
    //未获取数据异常
    return xData;
}

void IAudioPlay::clear() {
    framesMutex.lock();
    while(!frames.empty()){
        frames.front().drop();
        frames.pop_front();
    }
    framesMutex.unlock();
}
