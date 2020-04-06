//
// Created by 33136 on 2020-4-6.
//

#ifndef BOPLAY_FFPLAYERBUILDER_H
#define BOPLAY_FFPLAYERBUILDER_H


#include "IPlayerBuilder.h"

class FFPlayerBuilder : public IPlayerBuilder{
public:
    static void initHard(void *vm);

    static FFPlayerBuilder *get(){
        static FFPlayerBuilder ffPlayerBuilder;
        return &ffPlayerBuilder;
    }
protected:
    FFPlayerBuilder(){}
    virtual IDemux *createDemux();
    virtual IDecode *createDecode();
    virtual IResample *createResample();
    virtual IVideoView *createVideoView();
    virtual IAudioPlay *createAudioPlay();
    virtual IPlayer *createPlayer(unsigned char index = 0);

};


#endif //BOPLAY_FFPLAYERBUILDER_H
