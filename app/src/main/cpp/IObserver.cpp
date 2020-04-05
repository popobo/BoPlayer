//
// Created by 33136 on 2020-4-5.
//

#include "IObserver.h"

void IObserver::addObs(IObserver *observer) {
    if (!observer){
        return;;
    }
    mux.lock();
    observers.push_back(observer);
    mux.unlock();
}

void IObserver::Notify(XData xData) {
    mux.lock();
    //vector的size()每次调用的时候都会去统计一遍, 如果数量较大, 效率会降低
//    for (int i = 0; i < observers.size(); ++i) {
    for (auto & observer : observers) {
        observer->update(xData);
    }
    mux.unlock();
}
