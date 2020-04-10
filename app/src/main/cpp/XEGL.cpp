//
// Created by 33136 on 2020-4-5.
//

#include "XEGL.h"
#include <android/native_window_jni.h>
#include <EGL/egl.h>
#include <mutex>
#include "XLog.h"

class CXEGL : public XEGL{
public:
    EGLDisplay display = EGL_NO_DISPLAY;
    EGLSurface surface = EGL_NO_SURFACE;
    EGLContext context = EGL_NO_CONTEXT;
    std::mutex mux;

    virtual void close(){
        mux.lock();
        //把display和surface的绑定去掉
        if (display == EGL_NO_DISPLAY){
            mux.unlock();
            return;
        }
        eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (surface != EGL_NO_SURFACE){
            eglDestroySurface(display, surface);
        }
        if (context != EGL_NO_CONTEXT){
            eglDestroyContext(display, context);
        }
        eglTerminate(display);
        display = EGL_NO_DISPLAY;
        surface = EGL_NO_SURFACE;
        context = EGL_NO_CONTEXT;
        mux.unlock();
    }

    virtual void draw(){
        mux.lock();
        if (display == EGL_NO_DISPLAY || surface == EGL_NO_SURFACE){
            mux.unlock();
            XLOGE("display or surface is NULL");
            return;
        }
        eglSwapBuffers(display, surface);
        mux.unlock();
    }

    virtual bool init(void *win){
        ANativeWindow *anwin = (ANativeWindow *)win;
        close();
        mux.lock();
        //初始化EGL
        //1.获取EGL display对象 显示设备
        display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        if(EGL_NO_DISPLAY == display){
            mux.unlock();
            XLOGE("eglGetDisplay failed");
            return false;
        }
        XLOGI("eglGetDisplay successfully");
        //初始化display
        if(EGL_TRUE != eglInitialize(display, 0, 0)){
            mux.unlock();
            XLOGE("eglInitialize failed");
            return false;
        }
        XLOGI("eglInitialize successfully");
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
            mux.unlock();
            XLOGE("eglChooseConfig failed");
            return false;
        }
        XLOGI("eglChooseConfig successfully");

        surface = eglCreateWindowSurface(display, config, anwin, NULL);

        //4,创建并打开上下文
        const EGLint ctxAttr[] = {
                EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE
        };
        context = eglCreateContext(display, config, EGL_NO_CONTEXT, ctxAttr);
        if (EGL_NO_CONTEXT == context){
            mux.unlock();
            XLOGE("eglCreateContext failed");
            return false;
        }
        XLOGI("eglCreateContext successfully");

        if(EGL_TRUE != eglMakeCurrent(display, surface, surface, context)){
            XLOGE("eglMakeCurrent failed");
        }
        XLOGI("eglMakeCurrent successfully");
        mux.unlock();
        return true;
    }
};

//单例模式
XEGL *XEGL::get() {
    static CXEGL egl;
    return &egl;
}

void XEGL::close() {

}
