//
// Created by 33136 on 2020-4-4.
//

#include "FFDemux.h"
#include "XLog.h"
extern "C"{
#include "libavformat/avformat.h"
}

//分数转为浮点数
static  double r2d(AVRational r){
    return r.num == 0 || r.den == 0 ? 0.0 : (double)r.num/(double)r.den;
}

bool FFDemux::open(const char *url) {
    close();
    XLOGI("open url %s begin", url);
    icMutex.lock();
    int re = avformat_open_input(&ic, url, 0, 0);
    if (re != 0){
        icMutex.unlock();
        char buf[1024] = {0};
        av_strerror(re, buf, sizeof(buf));
        XLOGE("FFDemux open %s failed!", url);
        return false;
    }
    XLOGI("FFDemux open %s successfully!", url);

    //读取文件信息
    re = avformat_find_stream_info(ic, 0);
    if (re != 0){
        icMutex.unlock();
        char buf[1024] = {0};
        av_strerror(re, buf, sizeof(buf));
        XLOGE("FFDemux avformat_find_stream_info %s failed!", url);
        return false;
    }
    //duration时间计数单位, 一秒计数1000000次, 这个数值不一定有, 因为可能信息在流里,不在封装的文件头中
    this->totalMs = ic->duration/(AV_TIME_BASE)*1000;
    XLOGI("total ms = %d", totalMs);
    //在getVPara和getAPara之前解锁,因为这两个函数里可能还有锁, 锁两次会导致死锁
    icMutex.unlock();
    getVPara();
    getAPara();

    return true;
}

XParameter FFDemux::getVPara() {
    icMutex.lock();
    if(!ic){
        XLOGE("getVPara ic is null");
        return XParameter();
    }
    //获取了视频流索引
    int re =  av_find_best_stream(ic, AVMEDIA_TYPE_VIDEO, -1, -1, 0, 0);
    if (re < 0){
        icMutex.unlock();
        XLOGE("av_find_best_stream failed");
        return XParameter();
    }
    videoStream = re;
    XParameter xParameter;
    xParameter.para = ic->streams[re]->codecpar;
    icMutex.unlock();
    return xParameter;

}

XParameter FFDemux::getAPara() {
    icMutex.lock();
    if(!ic){
        XLOGE("getVPara ic is null");
        return XParameter();
    }
    //获取了视频流索引
    int re =  av_find_best_stream(ic, AVMEDIA_TYPE_AUDIO, -1, -1, 0, 0);
    if (re < 0){
        icMutex.unlock();
        XLOGE("av_find_best_stream failed");
        return XParameter();
    }
    audioStream = re;
    XParameter xParameter;
    xParameter.para = ic->streams[re]->codecpar;
    xParameter.channels = ic->streams[re]->codecpar->channels;
    xParameter.sample_rate = ic->streams[re]->codecpar->sample_rate;
    icMutex.unlock();
    return xParameter;
}

//读取一帧数据
XData FFDemux::Read() {
    icMutex.lock();
    if (!ic){
        return XData();
    }
    XData xData;
    AVPacket *pkt = av_packet_alloc();
    int re = av_read_frame(ic, pkt);
    if (re != 0){
        icMutex.unlock();
        av_packet_free(&pkt);
        return XData();
    }
//    XLOGI("pack size %d pts %lld", pkt->size, pkt->pts);
    xData.data = (unsigned char*)pkt;
    xData.size = pkt->size;

    if (pkt->stream_index == audioStream){
        xData.isAudio = true;
    }else if (pkt->stream_index == videoStream){
        xData.isAudio = false;
    } else{
        icMutex.unlock();
        av_packet_free(&pkt);
        return XData();
    }

    //转换pts(ms)
    pkt->pts = pkt->pts * (r2d(ic->streams[pkt->stream_index]->time_base)) * 1000;
    pkt->dts = pkt->dts * (r2d(ic->streams[pkt->stream_index]->time_base)) * 1000;
    icMutex.unlock();
    xData.pts = (int)pkt->pts;
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

void FFDemux::close() {
    icMutex.lock();
    if (ic){
        avformat_close_input(&ic);
    }
    icMutex.unlock();
}


