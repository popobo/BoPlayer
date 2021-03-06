//
// Created by 33136 on 2020-4-4.
//

#include "IDemux.h"
#include "XLog.h"

void IDemux::main() {
    while(!isExit){
        if (isPaused()){
            XSleep(1);
            continue;
        }

        XData xData = Read();
        if (xData.size > 0){
            //通知观察者, 如果没有观察者接受数据, 数据应该销毁
            notify(xData);
        } else{
            XSleep(1);
        }
//        XLOGI("IDemux Read %d", xData.size);
//        if(xData.size <= 0){
//            break;
//        }
    }
}
