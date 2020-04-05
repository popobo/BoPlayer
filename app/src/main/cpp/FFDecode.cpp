//
// Created by 33136 on 2020-4-5.
//

#include "FFDecode.h"
#include "XLog.h"

extern "C"{
#include "include/libavcodec/avcodec.h"
}

bool FFDecode::open(XParameter parameter) {

    if(nullptr == parameter.para){
        return false;
    }
    AVCodecParameters *para = parameter.para;
    //1.查找解码器
    AVCodec *avc = avcodec_find_decoder(para->codec_id);
    if(!avc){
        XLOGI("avcodec_find_decoder %d failed!", para->codec_id);
    }
    XLOGI("avcodec_find_decoder %d successfully!", para->codec_id);
    //2.创建解码器上下文, 并复制参数
    codecContext = avcodec_alloc_context3(avc);
    avcodec_parameters_to_context(codecContext, para);

    //多线程解码
    codecContext->thread_count = 8;
    //3.打开解码器
    int re = avcodec_open2(codecContext, 0, 0);
    if (re != 0){
        char buf[1024] = {0};
        av_strerror(re, buf, sizeof(buf) - 1);
        XLOGE("%s", buf);
        return false;
    }
    XLOGI("avcodec_open2 successfully!");

    if (AVMEDIA_TYPE_VIDEO == codecContext->codec_type){
        this->isAudio = false;
    } else if (AVMEDIA_TYPE_AUDIO == codecContext->codec_type){
        this->isAudio = true;
    }
    return true;
}

bool FFDecode::sendPacket(XData xData) {
    //临时变量
    if (xData.size <= 0 || !xData.data){
        return false;
    }

    //类成员, 多线程访问的变量
    if (!codecContext){
        return false;
    }

    int re = avcodec_send_packet(codecContext, (AVPacket *)xData.data);
    if (re != 0){
        return false;
    }

    return true;
}

XData FFDecode::recvFrame() {
    if (!codecContext){
        return XData();
    }
    if (!frame){
        frame = av_frame_alloc();
    }
    //再次调用会复用上次空间, 线程不安全
    int re = avcodec_receive_frame(codecContext, frame);
    if (re != 0){
        return XData();
    }
    XData xData;
    xData.data = (unsigned char*)frame;
    if (AVMEDIA_TYPE_VIDEO == codecContext->codec_type){
        xData.size = (frame->linesize[0] + frame->linesize[1] + frame->linesize[2]) * frame->height;
        xData.width = frame->width;
        xData.height = frame->height;
    }else if (AVMEDIA_TYPE_AUDIO == codecContext->codec_type){
        //样本字节数 * 单通道样本数 * 通道数
        xData.size = av_get_bytes_per_sample((AVSampleFormat)frame->format) * frame->nb_samples * frame->channels;
    }

    memcpy(xData.datas, frame->data, sizeof(xData.data));

    return xData;
}
