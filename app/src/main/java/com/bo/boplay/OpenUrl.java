package com.bo.boplay;

import android.content.pm.ActivityInfo;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

import androidx.appcompat.app.AppCompatActivity;

public class OpenUrl extends AppCompatActivity {
    private Button btFile;
    private Button btRtmp;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
        setContentView(R.layout.openurl);
        btFile = findViewById(R.id.playvideo);
        btFile.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v) {
                EditText text = findViewById(R.id.fileurl);
                //用户输入的URL, 打开视频
                open(text.getText().toString());

                //关闭当前窗口
                finish();
            }
        });

        btRtmp = findViewById(R.id.playrtmp);
        btRtmp.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v) {
                EditText text = findViewById(R.id.rtmpurl);
                //用户输入的URL, 打开视频
                open(text.getText().toString());

                //关闭当前窗口
                finish();
            }
        });
    }

    public native void open(String url);
}
