//
// Created by 33136 on 2020-4-6.
//

#ifndef BOPLAY_SLAUDIOPLAY_H
#define BOPLAY_SLAUDIOPLAY_H


#include "IAudioPlay.h"

class SLAudioPlay : public IAudioPlay{
public:
    virtual bool startPlay(XParameter xParameter);
    void playCall(void *bufQueue);
    virtual void close();

    SLAudioPlay();
    virtual ~SLAudioPlay();
protected:
    unsigned char *buf = 0;
    std::mutex mux;
};


#endif //BOPLAY_SLAUDIOPLAY_H
