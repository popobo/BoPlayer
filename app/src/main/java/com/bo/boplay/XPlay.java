package com.bo.boplay;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.Log;
import android.view.SurfaceHolder;
import android.util.AttributeSet;
import android.view.View;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;


public class XPlay extends GLSurfaceView implements SurfaceHolder.Callback, GLSurfaceView.Renderer, View.OnClickListener {

    public XPlay(Context context, AttributeSet attrs) {
        super(context, attrs);
        //android 8.0 以上的版本需要添加该语句才能正常播放
        setRenderer(this);
        setOnClickListener(this);
    }

    //每一次切换窗口surface都会销毁
    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        //初始化OpenGL EGL显示
        initView(holder.getSurface());
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) {
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
    }

    public native void initView(Object surface);

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {

    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {

    }

    @Override
    public void onDrawFrame(GL10 gl) {

    }


    @Override
    public void onClick(View v) {
        Log.e("XPlay", "Pause");
        playOrPause();
    }

    public native void playOrPause();
}
