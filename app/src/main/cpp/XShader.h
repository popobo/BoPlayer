//
// Created by 33136 on 2020-4-5.
//

#ifndef BOPLAY_XSHADER_H
#define BOPLAY_XSHADER_H


class XShader {
public:
    virtual bool init();
    //获取材质并映射到内存
    virtual void getTexture(unsigned int index, int width, int height, unsigned char *buf);

    virtual void draw();

protected:
    unsigned int vsh = 0;
    unsigned int fsh = 0;
    unsigned int program = 0;

    unsigned  int texts[100] = {0};
};


#endif //BOPLAY_XSHADER_H
