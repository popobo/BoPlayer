//
// Created by 33136 on 2020-4-5.
//

#include "IDecode.h"
#include "XLog.h"

//业务逻辑代码
void IDecode::main() {
    while(!isExit){
        xDataListMutex.lock();
        //判断音视频同步
        if (!isAudio && synPts > 0){
            //当音频时间小于视频时间时, 等音频
            if (synPts < pts){
                xDataListMutex.unlock();
                XSleep(1);
                continue;
            }
        }

        if (xDataList.empty()){
            xDataListMutex.unlock();
            XSleep(1);
            continue;
        }
        //取出packet 消费者
        XData xData = xDataList.front();
        xDataList.pop_front();

        //开始解码
        //发送数据到解码线程 一个数据包可能解码多个结果(主要是音频)
        if(sendPacket(xData)){
            while(!isExit){
                //获取解码数据
                XData frame = recvFrame();
                //解码音频会有许多frame.data == nullptr
                if (!frame.data){
                    break;
                }
                //注意这条语句的位置,有许多空数据
                pts = frame.pts;
                //发送数据给观察者
                this->notify(frame);
            }
        }
        //因为这是从xDataList取出的?
        xData.drop();
        xDataListMutex.unlock();
    }

}

void IDecode::update(XData xData) {
    //如果是音频则返回, 因为音频要重采样
    if (xData.isAudio != isAudio){
        return;
    }
    while(!isExit){
        xDataListMutex.lock();
        if (xDataList.size() < maxList){
            //生产者
            xDataList.push_back(xData);
            xDataListMutex.unlock();
            break;
        }
        xDataListMutex.unlock();
        XSleep(1);
    }

}

void IDecode::clear() {
    xDataListMutex.lock();
    while (!xDataList.empty()){
        xDataList.front().drop();
        xDataList.pop_front();
    }
    pts = 0;
    synPts = 0;

    xDataListMutex.unlock();
}
