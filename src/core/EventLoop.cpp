//
// Created by zhanglei on 2020/11/11.
//

#include "EventLoop.h"

bool Core::EventLoop::addEvent(int fd, int op) {
    struct epoll_event event;
    event.events
    event.data.ptr =
    epoll_ctl(eventPollFd, EPOLL_CTL_ADD, fd, &event);
}

bool Core::EventLoop::stop() {

}

bool Core::EventLoop::start() {

    if (eventPollFd == -1) {
        std::cerr << "epoll create error , error msg :" << strerror(errno) << std::endl;
    }

    while (state) {
        epoll_wait()
    }
}