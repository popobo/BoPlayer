//
// Created by 33136 on 2020-4-5.
//


extern "C"{
#include "libswresample/swresample.h"
#include "libavcodec/avcodec.h"
}
#include "FFResample.h"
#include "XLog.h"


bool FFResample::open(XParameter xParameterIn, XParameter xParameterOut) {
    close();
    actxMutex.lock();
    actx = swr_alloc();
    actx = swr_alloc_set_opts(actx,
                              av_get_default_channel_layout(xParameterOut.channels),
                              AV_SAMPLE_FMT_S16,
                              xParameterOut.sample_rate,
                              av_get_default_channel_layout(xParameterIn.para->channels),
                              (AVSampleFormat)xParameterIn.para->format,
                              xParameterIn.para->sample_rate, 0, 0);
    int re = swr_init(actx);
    if (re != 0){
        actxMutex.unlock();
        XLOGI("swr_init failed");
        return false;
    }
    actxMutex.unlock();
    outChannels = xParameterIn.para->channels;
    outFormat = AV_SAMPLE_FMT_S16;

    XLOGI("swr_init successfully");

    return true;
}

XData FFResample::resample(XData xDataIn) {
    //XLOGI("resample xDataIn.size = %d", xDataIn.size);
    if (xDataIn.size <= 0 || !xDataIn.data){
        return XData();
    }

    actxMutex.lock();
    if (!actx){
        actxMutex.unlock();
        return XData();
    }

    //输出空间的分配
    XData xDataOut;
    AVFrame *frame = (AVFrame *)xDataIn.data;
    //通道数 * 单通道样本数 * 样本字节大小
    int outSize = outChannels * frame->nb_samples * av_get_bytes_per_sample((AVSampleFormat)outFormat);
    if (outSize <= 0){
        actxMutex.unlock();
        return XData();
    }
    xDataOut.alloc(outSize);
    uint8_t *outArr[2] = {0};
    outArr[0] = xDataOut.data;
    int len = swr_convert(actx, outArr, frame->nb_samples, (const uint8_t **)frame->data, frame->nb_samples);
    if (len <= 0){
        actxMutex.unlock();
        xDataOut.drop();
        return XData();
    }
    actxMutex.unlock();
//    XLOGI("swr_convert successfully %d", len);
    xDataOut.pts = xDataIn.pts;
    return xDataOut;
}

void FFResample::close() {
    actxMutex.lock();
    if(actx){
        swr_free(&actx);
    }
    actxMutex.unlock();
}
