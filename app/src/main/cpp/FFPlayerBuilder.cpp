//
// Created by 33136 on 2020-4-6.
//

#include "FFPlayerBuilder.h"
#include "FFDemux.h"
#include "FFDecode.h"
#include "FFResample.h"
#include "GLVideoView.h"
#include "SLAudioPlay.h"

IDemux *FFPlayerBuilder::createDemux() {
    IDemux *demux = new FFDemux();
    return demux;
}

IDecode *FFPlayerBuilder::createDecode() {
    IDecode *decode = new FFDecode();
    return decode;
}

IResample *FFPlayerBuilder::createResample() {
    IResample *resample = new FFResample();
    return resample;
}

IVideoView *FFPlayerBuilder::createVideoView() {
    IVideoView *videoView = new GLVideoView();
    return videoView;
}

IAudioPlay *FFPlayerBuilder::createAudioPlay() {
    IAudioPlay *audioPlay = new SLAudioPlay();
    return audioPlay;
}

IPlayer *FFPlayerBuilder::createPlayer(unsigned char index) {
    return  IPlayer::get(index);
}
//因为FFPlayerBuilder本身就和FFDecode耦合, 所以ffmpeg的硬解码初始化放在这
void FFPlayerBuilder::initHard(void *vm) {
    FFDecode::initHard(vm);
}
