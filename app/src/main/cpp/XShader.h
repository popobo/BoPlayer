//
// Created by 33136 on 2020-4-5.
//

#ifndef BOPLAY_XSHADER_H
#define BOPLAY_XSHADER_H

//重新定义枚举
enum XShaderType{
    XSHADER_YUV420P = 0,//yyyy u v 软解码和虚拟机
    XSHADER_NV12 = 25,//yyyy uv 手机
    XSHADER_NV21 = 26//yyyy vu
};

class XShader {
public:
    virtual bool init(XShaderType type = XSHADER_YUV420P);
    //获取材质并映射到内存  isA是否有透明通道
    virtual void getTexture(unsigned int index, int width, int height, unsigned char *buf, bool isA = false);

    virtual void draw();

protected:
    unsigned int vsh = 0;
    unsigned int fsh = 0;
    unsigned int program = 0;

    unsigned  int texts[100] = {0};
};


#endif //BOPLAY_XSHADER_H
