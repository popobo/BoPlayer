//
// Created by 33136 on 2020-4-5.
//

#ifndef BOPLAY_IVIDEOVIEW_H
#define BOPLAY_IVIDEOVIEW_H


#include "XData.h"
#include "IObserver.h"

class IVideoView : public IObserver {
public:
    virtual void setRender(void *win) = 0;
    virtual void render(XData xData) = 0;
    virtual void update(XData xData);
    virtual void close() = 0;
};


#endif //BOPLAY_IVIDEOVIEW_H
