//
// Created by 33136 on 2020-4-4.
//

#ifndef BOPLAY_XLOG_H
#define BOPLAY_XLOG_H

#ifdef ANDROID
#include <android/log.h>
#define XLOGD(...) __android_log_print(ANDROID_LOG_DEBUG, "XPlay", __VA_ARGS__)
#define XLOGI(...) __android_log_print(ANDROID_LOG_INFO, "XPlay", __VA_ARGS__)
#define XLOGE(...) __android_log_print(ANDROID_LOG_ERROR, "XPlay", __VA_ARGS__)
#else
#define XLOGD(...) printf(ANDROID_LOG_DEBUG, "XPlay", __VA_ARGS__)
#define XLOGI(...) printf(ANDROID_LOG_INFO, "XPlay", __VA_ARGS__)
#define XLOGE(...) printf(ANDROID_LOG_ERROR, "XPlay", __VA_ARGS__)
#endif

class XLog {

};



#endif //BOPLAY_XLOG_H
