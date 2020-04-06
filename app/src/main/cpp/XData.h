//
// Created by 33136 on 2020-4-4.
//

#ifndef BOPLAY_XDATA_H
#define BOPLAY_XDATA_H

enum XDataType{
    AVPACKET_TYPE = 0,
    UNCHAR_TYPE = 1
};

struct XData {
    XDataType type = AVPACKET_TYPE;


    unsigned char *data = 0;
    unsigned char *datas[8] = {0};
    int size = 0;
    int width = 0;
    int height = 0;
    int format = 0;
    bool isAudio = false;
    bool alloc(int size, const char *dataIn = 0);
    void drop();
};


#endif //BOPLAY_XDATA_H
