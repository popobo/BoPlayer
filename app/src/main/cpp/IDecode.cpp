//
// Created by 33136 on 2020-4-5.
//

#include "IDecode.h"
#include "XLog.h"

void IDecode::main() {
    while(!isExit){
        xDataListMutex.lock();
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
                if (!frame.data){
                    break;
                }
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
