//
// Created by 33136 on 2020-4-5.
//

#ifndef BOPLAY_FFRESAMPLE_H
#define BOPLAY_FFRESAMPLE_H


#include "IResample.h"

struct SwrContext;

class FFResample : public IResample{
public:
    virtual bool open(XParameter xParameterIn, XParameter xParameterOut = XParameter());
    virtual XData resample(XData xDataIn);
protected:
    SwrContext *actx;
};


#endif //BOPLAY_FFRESAMPLE_H
