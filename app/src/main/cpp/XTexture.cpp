//
// Created by 33136 on 2020-4-5.
//

#include "XTexture.h"
#include "XLog.h"
#include "XEGL.h"
#include "XShader.h"

class CXTexture : public  XTexture{
public:
    XShader sh;
    XTextureType xTextureType;
    virtual bool init(void *win, XTextureType type){
        xTextureType = type;
        if(!win){
            XLOGE("CXTexture init failed win is NULL");
            return false;
        }
        if(!XEGL::get()->init(win)){
            return false;
        }
        sh.init((XShaderType)xTextureType);

        return true;
    }

    virtual void draw(unsigned char *data[], int width, int height){

        sh.getTexture(0, width, height, data[0]);//y
        switch (xTextureType){
            case XTEXTURE_YUV420P:
                sh.getTexture(1, width/2, height/2, data[1]);//u 一个像素点一个字节
                sh.getTexture(2, width/2, height/2, data[2]);//v
                break;
            case XTEXTURE_NV21:
            case XTEXTURE_NV12:
                sh.getTexture(1, width/2, height/2, data[1], true);//uv 一个像素点两个字节, 包含一个透明通道
                break;
            default:
                XLOGE("type %d is not supported", xTextureType);
                break;
        }

//        if(nullptr == data[1]){
//            XLOGE("nullptr == data[1]");
//        }
//        if(nullptr == data[2]){
//            XLOGE("nullptr == data[1]");
//        }
        sh.draw();
        XEGL::get()->draw();
    }
};

XTexture *XTexture::create() {
    return new CXTexture();
}
