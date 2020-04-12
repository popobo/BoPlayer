//
// Created by 33136 on 2020-4-7.
//

#include "IPlayerProxy.h"
#include "FFPlayerBuilder.h"
#include "XLog.h"

bool IPlayerProxy::open(const char *url) {
    bool re = false;
    mux.lock();
    if(player){
        re = player->open(url);
    }
    mux.unlock();
    return re;
}

bool IPlayerProxy::start() {
    bool re = false;
    mux.lock();
    if(player){
        re = player->start();
    }
    mux.unlock();
    return re;
}

void IPlayerProxy::initView(void *win) {
    bool re = false;
    mux.lock();
    if(player){
        player->initView(win);
    }
    mux.unlock();
}

void IPlayerProxy::init(void *vm) {
    mux.lock();
    if (vm){
        FFPlayerBuilder::initHard(vm);
    }
    if (!player){
        player = FFPlayerBuilder::get()->builderPlayer();
    }
    mux.unlock();
}

void IPlayerProxy::close() {
    mux.lock();
    if(player){
        player->close();
    }
    mux.unlock();
}
