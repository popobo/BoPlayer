//
// Created by 33136 on 2020-4-5.
//

#include "XEGL.h"
#include <android/native_window_jni.h>
#include <EGL/egl.h>
#include "XLog.h"

class CXEGL : public XEGL{
public:
    EGLDisplay display = EGL_NO_DISPLAY;
    EGLSurface surface = EGL_NO_SURFACE;
    EGLContext context = EGL_NO_CONTEXT;

    virtual bool init(void *win){
        ANativeWindow *anwin = (ANativeWindow *)win;
        //初始化EGL
        //1.获取EGL display对象 显示设备
        display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        if(EGL_NO_DISPLAY == display){
            XLOGE("eglGetDisplay failed");
            return false;
        }
        XLOGE("eglGetDisplay successfully");
        //初始化display
        if(EGL_TRUE != eglInitialize(display, 0, 0)){
            XLOGE("eglInitialize failed");
            return false;
        }
        XLOGE("eglInitialize successfully");
        //3.获取配置并创建surface
        EGLint configSpec[] = {
                EGL_RED_SIZE, 8,
                EGL_GREEN_SIZE, 8,
                EGL_BLUE_SIZE, 8,
                EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
                EGL_NONE
        };
        EGLConfig config;
        EGLint configNum;
        if(EGL_TRUE != eglChooseConfig(display, configSpec, &config, 1, &configNum)){
            XLOGE("eglChooseConfig failed");
            return false;
        }
        XLOGE("eglChooseConfig successfully");

        surface = eglCreateWindowSurface(display, config, anwin, NULL);

        //4,创建并打开上下文
        const EGLint ctxAttr[] = {
                EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE
        };
        context = eglCreateContext(display, config, EGL_NO_CONTEXT, ctxAttr);
        if (EGL_NO_CONTEXT == context){
            XLOGE("eglCreateContext failed");
            return false;
        }
        XLOGE("eglCreateContext successfully");

        if(EGL_TRUE != eglMakeCurrent(display, surface, surface, context)){
            XLOGE("eglMakeCurrent failed");
        }
        XLOGE("eglMakeCurrent successfully");

        return true;
    }
};

//单例模式
XEGL *XEGL::get() {
    static CXEGL egl;
    return &egl;
}
