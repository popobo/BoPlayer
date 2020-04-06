//
// Created by 33136 on 2020-4-6.
//

#include "IPlayerBuilder.h"
#include "IPlayer.h"
#include "IDemux.h"
#include "IDecode.h"
#include "IVideoView.h"
#include "IAudioPlay.h"
#include "IResample.h"

IPlayer *IPlayerBuilder::builderPlayer(unsigned char index) {
    IPlayer *iPlayer = createPlayer(index);
    IDemux *demux = createDemux();
    IDecode *vdecode = createDecode();
    IDecode *adecode = createDecode();
    IVideoView *view = createVideoView();
    IResample *resample = createResample();
    IAudioPlay *audioPlay = createAudioPlay();


    //添加观察者
    demux->addObs(vdecode);
    demux->addObs(adecode);
    vdecode->addObs(view);
    adecode->addObs(resample);
    resample->addObs(audioPlay);

    //注入
    iPlayer->demux = demux;
    iPlayer->videoDecoder= vdecode;
    iPlayer->audioDecoder = adecode;
    iPlayer->videoView = view;
    iPlayer->audioPlay = audioPlay;
    iPlayer->resample = resample;

    return iPlayer;
}
