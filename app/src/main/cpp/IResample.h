//
// Created by 33136 on 2020-4-5.
//

#ifndef BOPLAY_IRESAMPLE_H
#define BOPLAY_IRESAMPLE_H


#include "IObserver.h"
#include "XParameter.h"

class IResample : public IObserver{
public:
    virtual bool open(XParameter xParameterIn, XParameter xParameterOut = XParameter()) = 0;
    virtual XData resample(XData xDataIn) = 0;
    virtual void update(XData xDataIn);

    int outChannels = 2;
    int outFormat = 1;
};


#endif //BOPLAY_IRESAMPLE_H
