//
// Created by 33136 on 2020-4-5.
//

#include "XThread.h"
#include <thread>
#include "XLog.h"

//using namespace不要放在头文件中
using namespace std;

void XSleep(int ms){
    chrono::milliseconds sTime(ms);
    this_thread::sleep_for(sTime);
}

//启动线程
bool XThread::start() {
    isExit = false;
    thread th(&XThread::threadMain, this);
    //当前线程放弃对新建线程的控制, 防止对象被清空时, 新建线程出错
    th.detach();
    return true;
}

void XThread::stop() {
    isExit = true;
    for (int i = 0; i < 200; ++i) {
        XSleep(1);
        if (!isRunning){
            XLOGI("停止线程成功");
            return;
        }
        XSleep(1);
    }
    XLOGI("停止线程超时");
}

void XThread::threadMain() {
    XLOGI("线程函数进入");
    isRunning = true;
    main();
    isRunning = false;
    XLOGI("线程函数退出");
}


