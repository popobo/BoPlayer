//
// Created by 33136 on 2020-4-4.
//

#include "IDemux.h"
#include "XLog.h"

void IDemux::main() {
    while(!isExit){
        XData xData = Read();
//        XLOGI("IDemux Read %d", xData.size);
//        if(xData.size <= 0){
//            break;
//        }
    }
}
