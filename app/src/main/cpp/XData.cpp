//
// Created by 33136 on 2020-4-4.
//

#include "XData.h"
extern "C"{
#include "libavformat/avformat.h"
}

void XData::drop() {
    if(!data){
        return;
    }
    if (AVPACKET_TYPE == type){
        av_packet_free((AVPacket **)&data);
    } else if (UNCHAR_TYPE == type){
        delete data;
    }
    data = 0;
    size = 0;
}

bool XData::alloc(int size, const char *dataIn) {
    drop();
    type = UNCHAR_TYPE;
    if (size <= 0){
        return false;
    }
    data = new unsigned char[size];
    if (!data){
        return false;
    }

    if (dataIn){
        memcpy(data, dataIn, size);
    }
    this->size = size;
    return true;
}
