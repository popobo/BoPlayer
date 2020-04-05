//
// Created by 33136 on 2020-4-4.
//

#include "FFDemux.h"
#include "XLog.h"
extern "C"{
#include "libavformat/avformat.h"
}
bool FFDemux::Open(const char *url) {
    XLOGI("open url %s begin", url);
    int re = avformat_open_input(&ic, url, 0, 0);
    if (re != 0){
        char buf[1024] = {0};
        av_strerror(re, buf, sizeof(buf));
        XLOGE("FFDemux open %s failed!", url);
        return false;
    }
    XLOGI("FFDemux open %s successfully!", url);

    //读取文件信息
    re = avformat_find_stream_info(ic, 0);
    if (re != 0){
        char buf[1024] = {0};
        av_strerror(re, buf, sizeof(buf));
        XLOGE("FFDemux avformat_find_stream_info %s failed!", url);
        return false;
    }
    //duration时间计数单位, 一秒计数1000000次, 这个数值不一定有, 因为可能信息在流里,不在封装的文件头中
    this->totalMs = ic->duration/(AV_TIME_BASE)*1000;
    XLOGI("total ms = %d", totalMs);

    return true;
}

//读取一帧数据
XData FFDemux::Read() {
    if (!ic){
        return XData();
    }
    XData xData;
    AVPacket *pkt = av_packet_alloc();
    int re = av_read_frame(ic, pkt);
    if (re != 0){
        av_packet_free(&pkt);
        return XData();
    }
//    XLOGI("pack size %d pts %lld", pkt->size, pkt->pts);
    xData.data = (unsigned char*)pkt;
    xData.size = pkt->size;


    return xData;
}

FFDemux::FFDemux() {
    //这样不线程安全, 创建对象注意不要同时创建
    static bool isFirst = true;
    if (isFirst){
        isFirst = false;
        //注册所有封装器
        av_register_all();
        //注册所有解码器
        avcodec_register_all();
        //初始化网络
        avformat_network_init();
        XLOGI("register ffmpeg");
    }
}