//
// Created by zhanglei on 2020/11/21.
//

#ifndef PHPLOGGUARD_UNIXTIMER_H
#define PHPLOGGUARD_UNIXTIMER_H
#include <sys/timerfd.h>


namespace OS {
    class UnixTimer {
    public:
        int createTimer() {
            return timerfd_create(CLOCK_MONOTONIC,  TFD_NONBLOCK | TFD_CLOEXEC);
        }
    };
}


#endif //PHPLOGGUARD_UNIXTIMER_H
