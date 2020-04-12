//
// Created by 33136 on 2020-4-7.
//

#ifndef BOPLAY_IPLAYERPROXY_H
#define BOPLAY_IPLAYERPROXY_H


#include "IPlayer.h"
#include <mutex>

//这边继承时忘记添加public 导致父类的public方法变成private
class IPlayerProxy : public IPlayer{
public:
    static  IPlayerProxy *get(){
        static IPlayerProxy iPlayerProxy;
        return &iPlayerProxy;
    }
    void init(void *vm = nullptr);
    virtual bool open(const char *url);
    virtual bool start();
    virtual void initView(void  *win);
    virtual void close();
    virtual double playPos();
    virtual bool seek(double pos);
    virtual void setPause(bool isPauseIn);
    virtual bool isPaused();
protected:
    IPlayerProxy(){}
    IPlayer *player = nullptr;
    std::mutex mux;
};


#endif //BOPLAY_IPLAYERPROXY_H
