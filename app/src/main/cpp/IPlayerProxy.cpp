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
        //传递硬解码
        player->isHardDecoder = isHardDecoder;
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

double IPlayerProxy::playPos() {
    double pos = 0.0;
    mux.lock();
    if (player){
        pos = player->playPos();
    }
    mux.unlock();
    return pos;
}

bool IPlayerProxy::seek(double pos) {
    bool re = false;
    mux.lock();
    if (player){
        re = player->seek(pos);
    }
    mux.unlock();
    return re;
}

void IPlayerProxy::setPause(bool isPauseIn) {
    mux.lock();
    if(player){
        player->setPause(isPauseIn);
    }
    mux.unlock();
}

bool IPlayerProxy::isPaused() {
    bool re = false;
    mux.lock();
    if(player){
        re = player->isPaused();
    }
    mux.unlock();
    return re;
}
