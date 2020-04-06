//
// Created by 33136 on 2020-4-5.
//

#ifndef BOPLAY_XPARAMETER_H
#define BOPLAY_XPARAMETER_H

struct AVCodecParameters;

class XParameter {
public:
    //SLAudioPlay不能直接使用此处的channels否则便会与ffmpeg关联了, 增加耦合
    AVCodecParameters *para = nullptr;

    int channels = 2;
    int sample_rate = 44100;
};


#endif //BOPLAY_XPARAMETER_H
