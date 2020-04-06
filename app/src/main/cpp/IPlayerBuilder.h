//
// Created by 33136 on 2020-4-6.
//

#ifndef BOPLAY_IPLAYERBUILDER_H
#define BOPLAY_IPLAYERBUILDER_H

#include "IPlayer.h"

class IPlayerBuilder {
public:
    virtual IPlayer *builderPlayer(unsigned char index = 0);

protected:
    virtual IDemux *createDemux() = 0;
    virtual IDecode *createDecode() = 0;
    virtual IResample *createResample() = 0;
    virtual IVideoView *createVideoView() = 0;
    virtual IAudioPlay *createAudioPlay() = 0;
    virtual IPlayer *createPlayer(unsigned char index = 0) = 0;
};

#endif //BOPLAY_IPLAYERBUILDER_H
