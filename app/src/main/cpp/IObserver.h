//
// Created by 33136 on 2020-4-5.
//

#ifndef BOPLAY_IOBSERVER_H
#define BOPLAY_IOBSERVER_H

#include "XData.h"
#include "XThread.h"
#include <vector>
#include <mutex>

//观察者 和 主体
class IObserver : public XThread {
public:
    //观察者必须实现, 观察者接受数据函数, 主体调用观察者的update通知观察者已经收到数据了
    virtual void update(XData xData){}

    //主体函数, 添加观察者(线程安全)
    void addObs(IObserver *observer);

    //通知所有观察者(线程安全)
    void Notify(XData xData);

protected:
    std::vector<IObserver *> observers;
    std::mutex mux;
};


#endif //BOPLAY_IOBSERVER_H
