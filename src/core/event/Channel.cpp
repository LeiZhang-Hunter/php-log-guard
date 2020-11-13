//
// Created by zhanglei on 2020/11/11.
//

#include "Channel.h"
#include "EventLoop.h"

void Event::Channel::update() {
    loop->updateChannel(this);
}

bool Event::Channel::handelEvent() {
    /**
     * EPOLLIN 判断刻度
     * EPOLLPRI 判断外带数据
     * EPOLLRDHUP 对端关闭或者写一方关闭
     */
    if (events & (EPOLLIN | EPOLLPRI| EPOLLRDHUP)) {
        if (eventOnRead) eventOnRead();
    }

    if (!(events & EPOLLIN) && (events & EPOLLHUP)) {
        if (eventOnClose) eventOnClose();
    }

    if ((events & EPOLLERR)) {
        if (eventOnError) eventOnError();
    }

    if (events & EPOLLOUT) {
        if (eventOnWrite) eventOnWrite();
    }
    return true;
}