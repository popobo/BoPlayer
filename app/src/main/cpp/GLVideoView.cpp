//
// Created by 33136 on 2020-4-5.
//

#include "GLVideoView.h"
#include "XTexture.h"

void GLVideoView::setRender(void *win) {
    view = win;

}

void GLVideoView::render(XData xData) {
    if(!view){
        return;
    }

    if(!tex){
        tex = XTexture::create();
        tex->init(view);
    }

    tex->draw(xData.datas, xData.width, xData.height);

}
