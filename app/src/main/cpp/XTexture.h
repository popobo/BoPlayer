//
// Created by 33136 on 2020-4-5.
//

#ifndef BOPLAY_XTEXTURE_H
#define BOPLAY_XTEXTURE_H


class XTexture {
public:
    static XTexture *create();
    virtual bool init(void *win) = 0;
    virtual void draw(unsigned char *data[], int width, int height) = 0;
};


#endif //BOPLAY_XTEXTURE_H
