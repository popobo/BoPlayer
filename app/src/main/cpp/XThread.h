//
// Created by 33136 on 2020-4-5.
//

#ifndef BOPLAY_XTHREAD_H
#define BOPLAY_XTHREAD_H

//sleep 毫秒
void XSleep(int ms);

//c++ 11 线程库
class XThread {
public:
    //启动线程
    virtual bool start();
    //通过isExit变量安全停止线程(不一定成功), 在开发中不应该操作线程句柄直接让其停止, 风险大, 因为不知道程序执行到哪
    virtual void stop();
    //入口主函数
    virtual void main(){}

protected:
    bool isExit = false;
    bool isRunning = false;
private:
    void threadMain();
};


#endif //BOPLAY_XTHREAD_H
