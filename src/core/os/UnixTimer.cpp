//
// Created by zhanglei on 2020/11/21.
//

#include "os/UnixTimer.h"

void OS::UnixTimer::setInterval(int time) {
    struct itimerspec new_value;
    new_value.it_interval.tv_sec = time;
    new_value.it_interval.tv_nsec = 0;
    new_value.it_value.tv_sec = time;
    new_value.it_value.tv_nsec = 0;

    int res = timerfd_settime(timerFd, 0, &new_value, nullptr);
    if (res == -1) {
        std::cerr << strerror(errno) << std::endl;
        exit(-1);
    }
}

void OS::UnixTimer::confirm() {
    uint64_t exp;
    read(timerFd, &exp, sizeof(uint64_t));
}