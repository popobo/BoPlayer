//
// Created by 33136 on 2020-4-4.
//

#ifndef BOPLAY_XDATA_H
#define BOPLAY_XDATA_H


struct XData {
    unsigned char *data = 0;
    int size = 0;
    bool isAudio = false;
    void Drop();
};


#endif //BOPLAY_XDATA_H
