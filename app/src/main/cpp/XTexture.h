//
// Created by 33136 on 2020-4-5.
//

#ifndef BOPLAY_XTEXTURE_H
#define BOPLAY_XTEXTURE_H

enum XTextureType{
    //这边对应的是ffmpeg中video format枚举, 如果换成其他解码库则需要修改对应数据
    XTEXTURE_YUV420P = 0,//yyyy u v
    XTEXTURE_NV12 = 25,//yyyy uv
    XTEXTURE_NV21 = 26//yyyy vu
};

class XTexture {
public:
    static XTexture *create();
    virtual bool init(void *win, XTextureType type = XTEXTURE_YUV420P) = 0;
    virtual void draw(unsigned char *data[], int width, int height) = 0;
    virtual void drop() = 0;

    virtual ~XTexture(){};

protected:
    XTexture(){};
};


#endif //BOPLAY_XTEXTURE_H
