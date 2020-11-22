//
// Created by zhanglei on 2020/11/21.
//

#ifndef PHPLOGGUARD_UNIXTIMER_H
#define PHPLOGGUARD_UNIXTIMER_H
#include <sys/timerfd.h>
#include <unistd.h>

#include <cstdlib>
#include <iostream>
#include <cstring>

namespace OS {
    class UnixTimer {
    public:
        int createTimer() {
            timerFd = timerfd_create(CLOCK_MONOTONIC,  TFD_NONBLOCK | TFD_CLOEXEC);
            if (timerFd == -1) {
                std::cerr << strerror(errno) << std::endl;
                exit(-1);
            }
            return timerFd;
        }

        void setInterval(int time);

        /**
         * 确认消费时间
         */
        void confirm();

    private:
        int timerFd;
    };
}


#endif //PHPLOGGUARD_UNIXTIMER_H
