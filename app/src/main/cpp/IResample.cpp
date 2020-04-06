//
// Created by 33136 on 2020-4-5.
//

#include "IResample.h"

void IResample::update(XData xDataIn) {
    XData xDataOUT = resample(xDataIn);
    if (xDataOUT.size > 0){
        this->notify(xDataOUT);
    }
}
