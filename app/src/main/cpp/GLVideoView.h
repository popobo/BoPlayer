//
// Created by 33136 on 2020-4-5.
//

#ifndef BOPLAY_GLVIDEOVIEW_H
#define BOPLAY_GLVIDEOVIEW_H

#include "IVideoView.h"
class XTexture;

class GLVideoView : public IVideoView{
public:
    virtual void setRender(void *win);
    virtual void render(XData xData);
    virtual void close();
protected:
    void *view = nullptr;
    XTexture *tex = nullptr;
    std::mutex mux;
};


#endif //BOPLAY_GLVIDEOVIEW_H
