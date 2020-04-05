//
// Created by 33136 on 2020-4-5.
//

#ifndef BOPLAY_XEGL_H
#define BOPLAY_XEGL_H


class XEGL {
public:
    virtual bool init(void *win) = 0;
    static XEGL *get();
protected:
    XEGL(){}
};


#endif //BOPLAY_XEGL_H
