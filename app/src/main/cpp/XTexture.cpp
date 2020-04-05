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

    virtual bool init(void *win){
        if(!win){
            XLOGE("CXTexture init failed win is NULL");
            return false;
        }
        if(!XEGL::get()->init(win)){
            return false;
        }
        sh.init();


        return true;
    }

    virtual void draw(unsigned char *data[], int width, int height){

        sh.getTexture(0, width, height, data[0]);//y
        sh.getTexture(1, width/2, height/2, data[1]);//u
        sh.getTexture(2, width/2, height/2, data[2]);//v
        if(nullptr == data[1]){
            XLOGE("nullptr == data[1]");
        }
        if(nullptr == data[2]){
            XLOGE("nullptr == data[1]");
        }
        sh.draw();
        XEGL::get()->draw();
    }
};

XTexture *XTexture::create() {
    return new CXTexture();
}
