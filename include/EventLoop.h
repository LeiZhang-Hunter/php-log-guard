//
// Created by zhanglei on 2020/11/11.
//

#ifndef PHPLOGGUARD_EVENTLOOP_H
#define PHPLOGGUARD_EVENTLOOP_H
#include <sys/epoll.h>
#include <unistd.h>

#include <iostream>
#include <cstring>

#include "Noncopyable.h"

namespace Core {
    class EventLoop : public Noncopyable {
    public:
        EventLoop(size_t size) {
            eventPollFd = epoll_create1(EPOLL_CLOEXEC);
        };

        bool addEvent();

        bool stop();

        bool start();

        ~EventLoop() {
            ::close(eventPollFd);
        }

    private:
        bool state = true;
        int eventPollFd;
    };
}

#endif //PHPLOGGUARD_EVENTLOOP_H
