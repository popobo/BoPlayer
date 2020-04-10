//
// Created by 33136 on 2020-4-5.
//

#include "XShader.h"
#include "XLog.h"
#include <GLES2/gl2.h>

//顶点着色器代码glsl代码编写
#define  GET_STR(x) #x
static const char *vertexShader = GET_STR(
        attribute vec4 aPosition; //顶点信息
        attribute vec2 aTexCoord; //材质坐标
        varying vec2 vTexCoord; //输出的材质坐标
        void main(){
            vTexCoord = vec2(aTexCoord.x, 1.0 - aTexCoord.y);//以左上角为原点
            gl_Position = aPosition; //现实的顶点
        }
);

//像素(片元)着色器代码glsl代码编写, 软解码和部分x86硬解码格式yuv420p
static const char *fragYUV420P = GET_STR(
        precision mediump float; //精度
        varying vec2 vTexCoord; //顶点着色器传递的坐标
        uniform sampler2D yTexture;  //输入的材质(不透明灰度, 单像素)
        uniform sampler2D uTexture;
        uniform sampler2D vTexture;
        void main(){
            vec3 yuv;//vec3含有三个元素的向量
            vec3 rgb;
            //这变的rgb相当于yuv
            yuv.r = texture2D(yTexture,vTexCoord).r;
            yuv.g = texture2D(uTexture,vTexCoord).r - 0.5;
            //bug_fix:这边的vTexture写成了uTexture导致颜色出现偏差
            //yuv.b = texture2D(uTexture,vTexCoord).r - 0.5;
            yuv.b = texture2D(vTexture,vTexCoord).r - 0.5;
            rgb = mat3(1.0, 1.0, 1.0,
                       0.0, -0.39465, 2.03211,
                       1.13983, -0.58060, 0.0) * yuv;
            //输出像素颜色
            gl_FragColor = vec4(rgb, 1.0);
        }
);

//像素(片元)着色器代码glsl代码编写
static const char *fragNV12 = GET_STR(
        precision mediump float; //精度
        varying vec2 vTexCoord; //顶点着色器传递的坐标
        uniform sampler2D yTexture;  //输入的材质(不透明灰度, 单像素)
        uniform sampler2D uvTexture;
        void main(){
            vec3 yuv;//vec3含有三个元素的向量
            vec3 rgb;
            //这变的rgb相当于yuv
            yuv.r = texture2D(yTexture,vTexCoord).r;
            yuv.g = texture2D(uvTexture,vTexCoord).r - 0.5;//灰度存放了u
            yuv.b = texture2D(uvTexture,vTexCoord).a - 0.5;//透明度存放了v
            rgb = mat3(1.0, 1.0, 1.0,
                       0.0, -0.39465, 2.03211,
                       1.13983, -0.58060, 0.0) * yuv;
            //输出像素颜色
            gl_FragColor = vec4(rgb, 1.0);
        }
);

//像素(片元)着色器代码glsl代码编写
static const char *fragNV21 = GET_STR(
        precision mediump float; //精度
        varying vec2 vTexCoord; //顶点着色器传递的坐标
        uniform sampler2D yTexture;  //输入的材质(不透明灰度, 单像素)
        uniform sampler2D uvTexture;
        void main(){
            vec3 yuv;//vec3含有三个元素的向量
            vec3 rgb;
            //这变的rgb相当于yuv
            yuv.r = texture2D(yTexture,vTexCoord).r;
            yuv.g = texture2D(uvTexture,vTexCoord).a - 0.5;//灰度存放了u
            yuv.b = texture2D(uvTexture,vTexCoord).r - 0.5;//透明度存放了v
            rgb = mat3(1.0, 1.0, 1.0,
                       0.0, -0.39465, 2.03211,
                       1.13983, -0.58060, 0.0) * yuv;
            //输出像素颜色
            gl_FragColor = vec4(rgb, 1.0);
        }
);

static GLuint initShader(const char *code, GLint type){
    //创建shader
    GLuint sh = glCreateShader(type);
    if(0 == sh){
        XLOGI("glCreateShader %d falied", type);
        return 0;
    }
    //加载shader
    glShaderSource(sh,
                   1,//shader数量
                   &code,//shader代码
                   0);//代码长度, 传0则自动寻找结尾
    //编译shader让显卡运行
    glCompileShader(sh);
    //获取编译情况
    GLint status;
    glGetShaderiv(sh, GL_COMPILE_STATUS, &status);
    if (0 == status){
        XLOGI("glCompileShader failed!");
        return 0;
    }
    XLOGI("glCompileShader successfully!");
    return  sh;
}

bool XShader::init(XShaderType type) {
    close();
    mux.lock();
    //顶点和片元shader初始化
    //顶点shader初始化
    vsh = initShader(vertexShader, GL_VERTEX_SHADER);
    if (0 == vsh){
        mux.unlock();
        XLOGE("initShader GL_VERTEX_SHADER failed!");
        return false;
    }
    XLOGI("initShader GL_VERTEX_SHADER successfully!");
    switch (type){
        case XSHADER_YUV420P:
            //片元yuv420p shader初始化
            fsh = initShader(fragYUV420P, GL_FRAGMENT_SHADER);
            break;
        case XSHADER_NV12:
            fsh = initShader(fragNV12, GL_FRAGMENT_SHADER);
            break;
        case XSHADER_NV21:
            fsh = initShader(fragNV21, GL_FRAGMENT_SHADER);
            break;
        default:
            mux.unlock();
            XLOGE("XSHADER format is not supported");
            return false;
    }


    if (0 == fsh){
        mux.unlock();
        XLOGE("initShader GL_FRAGMENT_SHADER failed!");
        return false;
    }
    XLOGI("initShader GL_FRAGMENT_SHADER successfully!");

    //创建渲染程序
    program = glCreateProgram();
    if (0 == program){
        mux.unlock();
        XLOGE("glCreateProgram failed!");
        return false;
    }
    //渲染程序中加入着色器
    glAttachShader(program, vsh);
    glAttachShader(program, fsh);

    //链接程序
    glLinkProgram(program);
    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status != GL_TRUE){
        mux.unlock();
        XLOGE("glLinkProgram failed!");
        return false;
    }
    //激活渲染程序
    glUseProgram(program);

    //加入三维顶点数据 两个三角形组成正方形 用static保证程序运行过程中一直存在
    static float vers[] = {
            1.0F, -1.0F, 0.0F,
            -1.0F, -1.0F, 0.0F,
            1.0F, 1.0F, 0.0F,
            -1.0F, 1.0F, 0.0F
    };
    //获取shader当中的顶点变量,并使其有效
    GLuint apos = (GLuint)glGetAttribLocation(program, "aPosition");
    glEnableVertexAttribArray(apos);
    //传递顶点, xyz三个数据, 12(3个float是12个字节)是每一个值的间隔
    glVertexAttribPointer(apos, 3, GL_FLOAT, GL_FALSE, 12, vers);

    //加入材质坐标数据
    static float txts[] = {
            1.0F, 0.0F,//右下
            0.0F, 0.0F,
            1.0F, 1.0F,
            0.0, 1.0
    };
    GLuint atex = (GLuint)glGetAttribLocation(program, "aTexCoord");
    glEnableVertexAttribArray(atex);
    glVertexAttribPointer(atex, 2, GL_FLOAT, GL_FALSE, 8, txts);

    //材质纹理初始化
    //设置纹理层
    glUniform1i(glGetUniformLocation(program, "yTexture"), 0);//对应纹理第1层
    switch (type){
        case XSHADER_YUV420P:
            glUniform1i(glGetUniformLocation(program, "uTexture"), 1);//对应纹理第2层
            glUniform1i(glGetUniformLocation(program, "vTexture"), 2);//对应纹理第3层
            break;
        case XSHADER_NV21:
        case XSHADER_NV12:
            glUniform1i(glGetUniformLocation(program, "uvTexture"), 1);//对应纹理第2层
            break;
    }

    mux.unlock();
    XLOGI("初始化shader成功!");
    return true;
}

void XShader::getTexture(unsigned int index, int width, int height, unsigned char *buf, bool isA) {

    unsigned int format =  GL_LUMINANCE;

    if (isA){
        format = GL_LUMINANCE_ALPHA;
    }

    mux.lock();
    if (0 == texts[index]){
        //材质初始化
        glGenTextures(1, &texts[index]);
        //设置纹理属性
        glBindTexture(GL_TEXTURE_2D, texts[index]);//绑定,表示下面的属性是针对它来设置的
        //缩小时的过滤器, 线性插值
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        //放大过滤器
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //设置纹理的格式和大小
        glTexImage2D(GL_TEXTURE_2D,
                     0,//细节基本 0默认
                     format,//GPU内部格式,亮度,灰度图.y
                     width, height, //尺寸要是2的倍数, 默认拉伸到全屏
                     0, //边框
                     format, //数据的像素格式, 亮度, 灰度图, 要与上面一致
                     GL_UNSIGNED_BYTE, //像素的数据类型
                     NULL //纹理的数据
        );
    }

    //激活第index层纹理, 绑定到创建的openGL纹理
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D, texts[index]);
    //替换纹理内容
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, buf);
    mux.unlock();
}

void XShader::draw() {
    mux.lock();
    if (!program){
        mux.unlock();
        return;
    }

    //三维绘制 vers要保证存在
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    mux.unlock();
}

void XShader::close() {
    mux.lock();
    //释放shader
    if(program){
        glDeleteProgram(program);
    }
    if (fsh){
        glDeleteShader(fsh);
    }
    if (vsh){
        glDeleteShader(vsh);
    }
    //释放材质
    for (int i = 0; i < sizeof(texts)/ sizeof(unsigned int); i++) {
        if (texts[i]){
            glDeleteTextures(1,&texts[i]);
        }
        texts[i] = 0;
    }
    mux.unlock();
}
