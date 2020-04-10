//
// Created by 33136 on 2020-4-7.
//

#ifndef BOPLAY_IPLAYERPROXY_H
#define BOPLAY_IPLAYERPROXY_H


#include "IPlayer.h"
#include <mutex>

class IPlayerProxy : IPlayer{
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

protected:
    IPlayerProxy(){}
    IPlayer *player = nullptr;
    std::mutex mux;
};


#endif //BOPLAY_IPLAYERPROXY_H
